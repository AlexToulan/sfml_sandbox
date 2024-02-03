#include "EventTypes.hpp"
#include "GameOfLifeWorker.hpp"
#include "Utils/Logging.hpp"

GameOfLifeWorker::GameOfLifeWorker()
  : EventComponent()
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

void GameOfLifeWorker::init(int startX, int endX, int startY, int endY, int width, int height, std::vector<int>* cellNeighbors, std::vector<bool>* activeCells)
{
  _width = width;
  _height = height;
  _xRange = std::pair(startX, endX);
  _yRange = std::pair(startY, endY);
  _cellNeighbors = cellNeighbors;
  _activeCells = activeCells;
  _id = Event(_yRange);
}

void GameOfLifeWorker::start()
{
  if (_isRunning)
  {
    Log::error("thread already running");
  }
  subscribe(EventType::ACTIVATE_CELLS, &GameOfLifeWorker::setAliveDead);
  subscribe(EventType::CALC_NEIGHBORS, &GameOfLifeWorker::calcNeighbors);
  _isRunning = true;
  _thread = std::thread(&GameOfLifeWorker::run, this);
}

void GameOfLifeWorker::stop()
{
  if (_isRunning)
  {
    unsubscribe();
    _isRunning = false;
    _thread.join();
  }
}

void GameOfLifeWorker::run()
{
  while (_isRunning)
  {
    if (_setAliveDead)
    {
      classicRules();
      EventComponent::publish(EventType::ACTIVATE_CELLS_COMPLETE, _id);
      _setAliveDead = false;
    }
    if (_calcNeighbors)
    {
      int i = 0;
      for (int y = _yRange.first; y < _yRange.second; y++)
      {
        for (int x = _xRange.first; x < _xRange.second; x++)
        {
          (*_cellNeighbors)[i++] = calcNumNeighborsAlive(x, y);
        }
      }
      EventComponent::publish(EventType::CALC_NEIGHBORS_COMPLETE, _id);
      _calcNeighbors = false;
    }
  }
}

void GameOfLifeWorker::calcNeighbors(const EventBase& event)
{
  _calcNeighbors = true;
}

void GameOfLifeWorker::setAliveDead(const EventBase& event)
{
  _setAliveDead = true;
}

int GameOfLifeWorker::wrap(int value, int min, int max)
{
  if (value < min)
    return max;
  if (value > max)
    return min;
  return value;
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
      if ((*_activeCells)[getCellIndex(wrap(nx, 0, _width - 1), wrap(ny, 0, _height - 1))])
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
      if ((*_activeCells)[cellIndex])
      {
        if ((*_cellNeighbors)[cellIndex] < 2)
          (*_activeCells)[cellIndex] = false;
        if ((*_cellNeighbors)[cellIndex] > 3)
          (*_activeCells)[cellIndex] = false;
      }
      else
      {
        if ((*_cellNeighbors)[cellIndex] == 3)
          (*_activeCells)[cellIndex] = true;
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
      if ((*_activeCells)[cellIndex])
      {
        if ((*_cellNeighbors)[cellIndex] < lower)
          (*_activeCells)[cellIndex] = false;
        if ((*_cellNeighbors)[cellIndex] > upper)
          (*_activeCells)[cellIndex] = false;
      }
      else
      {
        if ((*_cellNeighbors)[cellIndex] >= lower && (*_cellNeighbors)[cellIndex] <= upper)
          (*_activeCells)[cellIndex] = true;
      }
    }
  }
}
