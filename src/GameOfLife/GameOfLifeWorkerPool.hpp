#pragma once

#include "Utils/EventSystem/EventComponent.hpp"
#include "Utils/ThreadWorker.hpp"

class GameOfLifeWorkerPool : public EventComponent, public ThreadWorker
{
public:
  GameOfLifeWorkerPool();
  GameOfLifeWorkerPool (const GameOfLifeWorkerPool&) = delete;
  virtual ~GameOfLifeWorkerPool();

private:
  std::vector<bool> _activeCells;
  std::vector<int> _cellNeighbors;
};
