#pragma once

#include <thread>

#include "Utils/EventSystem/EventComponent.hpp"

class GameOfLifeWorker : public EventComponent
{
public:
  GameOfLifeWorker();
  virtual ~GameOfLifeWorker();
  void init(int startX, int endX, int startY, int endY, int width, int height, std::vector<int>* cellNeighbors, std::vector<bool>* activeCells);
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

  std::vector<int>* _cellNeighbors;
  std::vector<bool>* _activeCells;

  std::thread _thread;
  std::atomic<bool> _isRunning;
  bool _calcNeighbors;
  bool _setAliveDead;
  Event<std::pair<int, int>> _id;
};
