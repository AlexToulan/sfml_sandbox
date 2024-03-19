#pragma once

#include "Utils/EventSystem/EventListener.hpp"
// #include "GameOfLifeWorker.hpp"
#include "GameMode/GameMode.hpp"
#include "UI/CellGrid.hpp"

#include <condition_variable>
#include <thread>

class GameOfLife : public EventListener, public GameMode
{
public:
  GameOfLife();
  virtual ~GameOfLife();
  virtual void onStart();
  virtual sf::Vector2f onResize(int screenX, int screenY);
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  // helper methods
  void basicSeed();
  bool seedFromConfig(std::string configName);
  void setSeed(const std::vector<bool>& seed, int width, int height, int center_x, int center_y);
  void saveSeed(const std::vector<bool>& seed, int width, int height, std::string fileName);
  bool getCell(int x, int y) const;
  void setCell(int x, int y, bool alive);
  void setCell(int i, bool alive);

  void startThreads();
  void classicRules(int startY, int endY);
  void calcNumNeighborsAlive(int startY, int endY);
  int getNumNeighborsAlive(int x, int y);
  void activateCellsComplete();
  void calcNeighborsComplete();

  // threads
  std::vector<std::thread> _threads;
  int _numThreads;
  int _rowsPerThread;
  std::condition_variable cv;
  std::mutex cvm;
  bool _bThreadsStarted;
  std::atomic<int> _calcNeighborsComplete;

  // grid
  CellGrid _cellGrid;
  std::unique_ptr<sf::Color[]> _swatch;
  std::unique_ptr<bool[]> _activeCells;
  std::unique_ptr<uint8_t[]> _cellNeighbors;
  size_t _numCells;
  size_t _numCellsX;
  size_t _numCellsY;

  // pause
  bool _bIsPaused;
  float _startDelaySec;
  bool _bStartDelayComplete;

  // input
  bool _bPauseKey;
  bool _bStepKeyPressed;
};
