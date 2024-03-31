#pragma once

#include <memory>
#include <thread>

#include "Utils/EventSystem/EventListener.hpp"
#include "Utils/Timer.hpp"

class GameOfLifeWorker : public EventListener
{
public:
  GameOfLifeWorker();
  virtual ~GameOfLifeWorker();
  void init(std::pair<int, int> yRange, int width, int height,
     const std::shared_ptr<bool[]>& activeCells, const  std::shared_ptr<uint8_t[]>& cellNeighbors);
  void start();
  void stop();
  void setTargetLoopTime(std::chrono::microseconds targetLoopTime);

private:
  // thread and events
  void run();
  void calcNeighbors();
  void setAliveDead();
  void togglePause(const bool& bPause);

  // helper methods
  void setCellsClassicRules();
  void calcNumNeighborsAlive();
  int getNumNeighborsAlive(int x, int y);
  size_t getCellIndex(int x, int y) const;
  void classicRules();
  void crazyRules();

  int _width;
  int _height;
  std::pair<int, int> _yRange;

  std::shared_ptr<bool[]> _activeCells;
  std::shared_ptr<uint8_t[]> _cellNeighbors;
  size_t _numCells;

  std::thread _thread;
  std::atomic<bool> _isRunning;
  std::atomic<bool> _bPaused;
  bool _calcNeighbors;
  bool _setAliveDead;
  bool _bShouldSleep;
  Timer _loopTimer;
  std::chrono::microseconds _targetLoopTime;
};
