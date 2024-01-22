#pragma once

#include "Utils/GameMode/GameMode.hpp"
#include "Utils/UI/CellGrid.hpp"

class GameOfLifeGameMode : public GameMode
{
public:
  GameOfLifeGameMode();
  virtual ~GameOfLifeGameMode();
  virtual void onStart();
  virtual void processEvents(sf::Event& event);
  virtual void update(float delta_seconds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  void basicSeed();
  bool getCell(int x, int y);
  void setCell(int x, int y, bool alive);
  int wrap(int value, int min, int max);
  int calcNumNeighborsAlive(int x, int y);

  sf::Color* _swatch;
  CellGrid _cellGrid;

  float _currentUpdateSec;
  float _secPerUpdate;
  std::vector<bool> _activeCells;
  std::vector<int> _cellNeighbors;
  bool _bIsPaused;

  // input
  bool _bPauseKey;
};
