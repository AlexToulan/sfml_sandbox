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
  void init(int startX, int endX, int startY, int endY, int width, int height,
     const std::shared_ptr<bool[]>& activeCells, const  std::shared_ptr<uint8_t[]>& cellNeighbors);
  void start();
  void stop();

private:
  // thread and events
  void run();
  void calcNeighbors();
  void setAliveDead();
  void togglePause(const bool& bPause);
  void secPerUpdate(const float& secPerUpdate);

  // helper methods
  int calcNumNeighborsAlive(int x, int y);
  size_t getCellIndex(int x, int y) const;
  void classicRules();
  void crazyRules();

  int _width;
  int _height;
  std::pair<int, int> _xRange;
  std::pair<int, int> _yRange;

  std::shared_ptr<bool[]> _activeCells;
  std::shared_ptr<uint8_t[]> _cellNeighbors;
  size_t _numCells;

  std::pair<int, int> _id;
  std::thread _thread;
  std::atomic<bool> _isRunning;
  std::atomic<bool> _bPaused;
  bool _calcNeighbors;
  bool _setAliveDead;
  bool _bShouldSleep;
  Timer _loopTimer;
  std::chrono::microseconds _targetLoopTime;
};
