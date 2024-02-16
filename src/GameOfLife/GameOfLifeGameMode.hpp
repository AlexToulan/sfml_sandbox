#pragma once

#include "Utils/EventSystem/EventComponent.hpp"
#include "GameOfLifeWorker.hpp"
#include "GameMode/GameMode.hpp"
#include "UI/CellGrid.hpp"

class GameOfLifeGameMode : public GameMode, public EventComponent
{
public:
  GameOfLifeGameMode();
  virtual ~GameOfLifeGameMode();
  virtual void onStart();
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  // game rules
  void classicRules();
  void crazyRules();

  // helper methods
  void basicSeed();
  bool getCell(int x, int y);
  void setCell(int x, int y, bool alive);

  void startWorkers(int width, int height);
  void activateCellsComplete(const EventBase& event);
  void calcNeighborsComplete(const EventBase& event);
  
  std::atomic<int> _rowsProcessed;
  std::vector<std::unique_ptr<GameOfLifeWorker>> _workers;

  std::unique_ptr<sf::Color[]> _swatch;
  CellGrid _cellGrid;
  std::mutex _cellMutex; // prevents screen tearing
  // TODO: use this once we have C++20
  // std::shared_ptr<bool[]> _activeCells;
  // std::shared_ptr<int[]> _cellNeighbors;
  bool* _activeCells;
  int* _cellNeighbors;
  size_t _numCells;
  bool _bIsPaused;

  // input
  bool _bPauseKey;
};
