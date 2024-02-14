#pragma once

#include <thread>

#include "Utils/EventSystem/EventComponent.hpp"

class GameOfLifeWorker : public EventComponent
{
public:
  GameOfLifeWorker();
  virtual ~GameOfLifeWorker();
  void init(int startX, int endX, int startY, int endY, int width, int height,
    std::shared_ptr<bool[]> activeCells, std::shared_ptr<int[]> cellNeighbors);
  void start();
  void stop();

private:
  // thread and events
  void run();
  void calcNeighbors(const EventBase& event);
  void setAliveDead(const EventBase& event);
  // helper methods
  int wrap(int value, int min, int max);
  int calcNumNeighborsAlive(int x, int y);
  size_t getCellIndex(int x, int y) const;
  void classicRules();
  void crazyRules();

  int _width;
  int _height;
  std::pair<int, int> _xRange;
  std::pair<int, int> _yRange;

  std::shared_ptr<bool[]> _activeCells;
  std::shared_ptr<int[]> _cellNeighbors;
  size_t _numCells;

  std::thread _thread;
  std::atomic<bool> _isRunning;
  bool _calcNeighbors;
  bool _setAliveDead;
  Event<std::pair<int, int>> _id;
};
