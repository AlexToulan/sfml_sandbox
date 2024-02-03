#pragma once

#include "GameOfLifeWorker.hpp"
#include "GameMode/GameMode.hpp"
#include "UI/CellGrid.hpp"

class GameOfLifeGameMode : public GameMode
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
  int wrap(int value, int min, int max);
  int calcNumNeighborsAlive(int x, int y);

  std::vector<GameOfLifeWorker> _workers;

  std::unique_ptr<sf::Color[]> _swatch;
  CellGrid _cellGrid;
  std::vector<bool> _activeCells;
  std::vector<int> _cellNeighbors;
  bool _bIsPaused;

  // input
  bool _bPauseKey;
};
