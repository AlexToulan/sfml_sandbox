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
  _bShouldSleep = false;
}

GameOfLifeWorker::~GameOfLifeWorker()
{
  stop();
}

void GameOfLifeWorker::init(std::pair<int, int> yRange, int width, int height,
  const std::shared_ptr<bool[]>& activeCells, const std::shared_ptr<uint8_t[]>& cellNeighbors)
{
  _width = width;
  _height = height;
  _yRange = yRange;
  _activeCells = activeCells;
  _cellNeighbors = cellNeighbors;
  _targetLoopTime = std::chrono::microseconds(10);
  togglePause(false);
}

void GameOfLifeWorker::start()
{
  if (_isRunning)
  {
    Log().error("can't start a thread that's already running");
    return;
  }
  Events::Game->bind(EGameEvent::CALC_NEIGHBORS, this, &GameOfLifeWorker::calcNeighbors);
  Events::Game->bind(EGameEvent::ACTIVATE_CELLS, this, &GameOfLifeWorker::setAliveDead);
  Events::Game->bind(EGameEvent::TOGGLE_PAUSE, this, &GameOfLifeWorker::togglePause);
  _isRunning = true;
  _loopTimer.reset();
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
  else
  {
    Log().error("can't stop a thread that isn't running");
  }
}

void GameOfLifeWorker::setTargetLoopTime(std::chrono::microseconds targetLoopTime)
{
  _targetLoopTime = targetLoopTime;
}   

void GameOfLifeWorker::run()
{
  while (_isRunning)
  {
    if (_bPaused)
    {
      std::this_thread::sleep_for(_targetLoopTime);
      continue;
    }
    if (_calcNeighbors)
    {
      _loopTimer.start();
      calcNumNeighborsAlive();
      Events::Game->publish(EGameEvent::CALC_NEIGHBORS_COMPLETE);
      _calcNeighbors = false;
    }
    if (_setAliveDead)
    {
      setCellsClassicRules();
      // crazyRules();
      Events::Game->publish(EGameEvent::ACTIVATE_CELLS_COMPLETE);
      _setAliveDead = false;
      _bShouldSleep = true;
      _loopTimer.stop();
    }
    if (_bShouldSleep)
    {
      _bShouldSleep = false;
      std::this_thread::sleep_for(_targetLoopTime - _loopTimer.getMicroSecDuration() - std::chrono::milliseconds(1));
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

void GameOfLifeWorker::togglePause(const bool& bPause)
{
  _bPaused = bPause;
}

void GameOfLifeWorker::calcNumNeighborsAlive()
{
  int i = _yRange.first * _width;
  for (int y = _yRange.first; y < _yRange.second; y++)
  {
    for (int x = 0; x < _width; x++)
    {
      _cellNeighbors[i++] = getNumNeighborsAlive(x, y);
    }
  }
}

int GameOfLifeWorker::getNumNeighborsAlive(int x, int y)
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

void GameOfLifeWorker::setCellsClassicRules()
{
  for (auto i = _yRange.first * _width; i < _yRange.second * _width; i++)
  {
    if (_activeCells[i])
    {
      if (_cellNeighbors[i] < 2 || _cellNeighbors[i] > 3)
        _activeCells[i] = false;
    }
    else
    {
      if (_cellNeighbors[i] == 3)
        _activeCells[i] = true;
    }
  }
}

void GameOfLifeWorker::crazyRules()
{
  int lower = 2;
  int upper = 3;
  for (int y = _yRange.first; y < _yRange.second; y++)
  {
    for (int x = 0; x < _width; x++)
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
