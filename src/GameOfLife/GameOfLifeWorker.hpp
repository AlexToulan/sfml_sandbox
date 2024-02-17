#pragma once

#include <thread>

#include "Utils/EventSystem/EventComponent.hpp"

class GameOfLifeWorker : public EventComponent
{
public:
  GameOfLifeWorker();
  virtual ~GameOfLifeWorker();
  void init(int startX, int endX, int startY, int endY, int width, int height,
    bool* activeCells, int* cellNeighbors);
  void start();
  void stop();

private:
  // thread and events
  void run();
  void calcNeighbors(const EventBase& event);
  void setAliveDead(const EventBase& event);
  // helper methods
  int calcNumNeighborsAlive(int x, int y);
  size_t getCellIndex(int x, int y) const;
  void classicRules();
  void crazyRules();

  int _width;
  int _height;
  std::pair<int, int> _xRange;
  std::pair<int, int> _yRange;

  // TODO: use this once we have C++20
  // std::shared_ptr<bool[]> _activeCells;
  // std::shared_ptr<int[]> _cellNeighbors;
  bool* _activeCells;
  int* _cellNeighbors;
  size_t _numCells;

  std::thread _thread;
  std::atomic<bool> _isRunning;
  bool _calcNeighbors;
  bool _setAliveDead;
  Event<std::pair<int, int>> _id;
};
