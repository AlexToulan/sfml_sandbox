#pragma once

#include "GameOfLifeWorker.hpp"
#include "GameMode/GameMode.hpp"
#include "UI/CellGrid.hpp"
#include "Utils/EventSystem/EventListener.hpp"
#include "Utils/Timer.hpp"

#include <condition_variable>
#include <memory>
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
  // threads
  void startThreads();
  void stopThreads();

  // events
  void calcNeighborComplete();
  void setCellsComplete();

  // helper methods
  void updateCellGrid();
  void basicSeed();
  bool seedFromConfig(std::string configName, int offsetX = 0, int offsetY = 0);
  void setSeed(const std::vector<bool>& seed, int width, int height, int center_x, int center_y);
  void saveSeed(const std::vector<bool>& seed, int width, int height, std::string fileName);
  bool getCell(int x, int y) const;
  void setCell(int x, int y, bool alive);
  void setCell(int i, bool alive);

  // threads
  std::vector<std::unique_ptr<GameOfLifeWorker>> _threads;
  std::vector<std::pair<int, int>> _threadParams;
  bool _bThreadsStarted;
  bool _bShutdownThreads;
  size_t _numThreads;
  int _rowsPerThread;
  std::atomic<int> _numCalcNeighborComplete;
  std::atomic<int> _numSetCellsComplete;
  Timer _loopTimer;
  std::chrono::microseconds _lastLoopTime;

  // grid
  std::mutex _cellGridLock;
  CellGrid _cellGrid;
  sf::Color _inactiveColor;
  sf::Color _activeColor;
  std::shared_ptr<bool[]> _activeCells;
  std::shared_ptr<uint8_t[]> _cellNeighbors;
  size_t _numCells;
  size_t _numCellsX;
  size_t _numCellsY;

  // pause
  bool _bIsPaused;
  float _startDelaySec;
  bool _bStartDelayComplete;

  // input
  bool _bPauseKey;
};
