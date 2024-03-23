#pragma once

#include "Utils/EventSystem/EventListener.hpp"
// #include "GameOfLifeWorker.hpp"
#include "GameMode/GameMode.hpp"
#include "UI/CellGrid.hpp"
#include "Utils/Timer.hpp"

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
  bool seedFromConfig(std::string configName, int offsetX = 0, int offsetY = 0);
  void setSeed(const std::vector<bool>& seed, int width, int height, int center_x, int center_y);
  void saveSeed(const std::vector<bool>& seed, int width, int height, std::string fileName);
  bool getCell(int x, int y) const;
  void setCell(int x, int y, bool alive);
  void setCell(int i, bool alive);

  void startThreads();
  void stopThreads();
  void classicRules(int startY, int endY);
  void threadSync();
  void calcNumNeighborsAlive(int startY, int endY);
  int getNumNeighborsAlive(int x, int y);
  void setCellsClassicRules(int startY, int endY);

  // threads
  std::thread _threadSync;
  std::vector<std::thread> _threads;
  std::vector<std::pair<int, int>> _threadParams;
  bool _bThreadsStarted;
  bool _bShutdownThreads;
  size_t _numThreads;
  int _rowsPerThread;

  std::mutex cnMut;
  std::mutex scMut;
  std::condition_variable cnCv;
  std::condition_variable scCv;
  std::atomic<int> _threadsWorking;
  int _threadsCalcNeighbors;
  int _threadsSettingCells;
  bool _calcNeighborComplete;
  bool _setCellsComplete;
  std::map<int, Timer> _threadTimers;
  Timer _loopTimer;
  std::chrono::microseconds _lastLoopTime;

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
