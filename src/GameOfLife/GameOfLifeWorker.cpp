#include "GameMode/GameEvents.hpp"
#include "GameOfLifeWorker.hpp"
#include "Utils/Logging.hpp"
#include "Utils/MathUtils.hpp"

GameOfLifeWorker::GameOfLifeWorker()
  : EventListener()
  , _thread()
{
  _isRunning = false;
  _calcNeighbors = false;
  _setAliveDead = false;
}

GameOfLifeWorker::~GameOfLifeWorker()
{
  stop();
}

void GameOfLifeWorker::init(int startX, int endX, int startY, int endY, int width, int height,
  bool* activeCells, int* cellNeighbors)
{
  _width = width;
  _height = height;
  _xRange = std::pair(startX, endX);
  _yRange = std::pair(startY, endY);
  _cellNeighbors = cellNeighbors;
  _activeCells = activeCells;
  _id = _yRange;
}

void GameOfLifeWorker::start()
{
  if (_isRunning)
  {
    Log::error("thread already running");
  }
  Events::Game->bind(EGameEvent::ACTIVATE_CELLS, this, &GameOfLifeWorker::setAliveDead);
  Events::Game->bind(EGameEvent::CALC_NEIGHBORS, this, &GameOfLifeWorker::calcNeighbors);
  _isRunning = true;
  _thread = std::thread(&GameOfLifeWorker::run, this);
}

void GameOfLifeWorker::stop()
{
  if (_isRunning)
  {
    Events::Game->unsubscribe(this);
    _isRunning = false;
    _thread.join();
  }
}

void GameOfLifeWorker::run()
{
  while (_isRunning)
  {
    if (_calcNeighbors)
    {
      for (int y = _yRange.first; y < _yRange.second; y++)
      {
        for (int x = _xRange.first; x < _xRange.second; x++)
        {
          _cellNeighbors[getCellIndex(x, y)] = calcNumNeighborsAlive(x, y);
        }
      }
      Events::Game->publish(EGameEvent::CALC_NEIGHBORS_COMPLETE, _id);
      _calcNeighbors = false;
    }
    if (_setAliveDead)
    {
      classicRules();
      // crazyRules();
      Events::Game->publish(EGameEvent::ACTIVATE_CELLS_COMPLETE, _id);
      _setAliveDead = false;
    }
  }
}

void GameOfLifeWorker::calcNeighbors()
{
  _calcNeighbors = true;
}

void GameOfLifeWorker::setAliveDead()
{
  _setAliveDead = true;
}

int GameOfLifeWorker::calcNumNeighborsAlive(int x, int y)
{
  // wrap field!
  int numNeighborsAlive = 0;
  for (int ny = y - 1; ny <= y + 1; ny++)
  {
    for (int nx = x - 1; nx <= x + 1; nx++)
    {
      if (nx == x && ny == y)
        continue;
      if (_activeCells[getCellIndex(mu::wrap(nx, 0, _width - 1), mu::wrap(ny, 0, _height - 1))])
        numNeighborsAlive++;
    }
  }
  return numNeighborsAlive;
}

size_t GameOfLifeWorker::getCellIndex(int x, int y) const
{
  return y * _width + x;
}

void GameOfLifeWorker::classicRules()
{
  for (int y = _yRange.first; y < _yRange.second; y++)
  {
    for (int x = _xRange.first; x < _xRange.second; x++)
    {
      int cellIndex = getCellIndex(x, y);
      if (_activeCells[cellIndex])
      {
        if (_cellNeighbors[cellIndex] < 2)
          _activeCells[cellIndex] = false;
        if (_cellNeighbors[cellIndex] > 3)
          _activeCells[cellIndex] = false;
      }
      else
      {
        if (_cellNeighbors[cellIndex] == 3)
          _activeCells[cellIndex] = true;
      }
    }
  }
}

void GameOfLifeWorker::crazyRules()
{
  int lower = 2;
  int upper = 3;
  for (int y = _yRange.first; y < _yRange.second; y++)
  {
    for (int x = _xRange.first; x < _xRange.second; x++)
    {
      int cellIndex = getCellIndex(x, y);
      if (_activeCells[cellIndex])
      {
        if (_cellNeighbors[cellIndex] < lower)
          _activeCells[cellIndex] = false;
        if (_cellNeighbors[cellIndex] > upper)
          _activeCells[cellIndex] = false;
      }
      else
      {
        if (_cellNeighbors[cellIndex] >= lower && _cellNeighbors[cellIndex] <= upper)
          _activeCells[cellIndex] = true;
      }
    }
  }
}
