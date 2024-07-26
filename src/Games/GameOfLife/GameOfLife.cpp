#include "GameOfLife.hpp"
#include "GameEvents.hpp"
#include "Utils/Logging.hpp"
#include "Utils/MathUtils.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Yaml.hpp"

#include <algorithm>
#include <shared_mutex>
#include <source_location>

std::unique_ptr<Events::GameEventSystem> Events::Game;

GameOfLife::GameOfLife(sf::Vector2u originalScreenSize)
 : EventListener()
 , GameMode(originalScreenSize)
 , _activeCells(nullptr)
 , _cellNeighbors(nullptr)
{
  Events::Game = std::make_unique<Events::GameEventSystem>("GameEvents");
  _numCellsX = 200;
  _numCellsY = 200;
  _numThreads = std::max(std::thread::hardware_concurrency() / 2, 2u);
  _threads.reserve(_numThreads);
  _threadParams.reserve(_numThreads);
  _rowsPerThread = _numCellsY / _numThreads;
  auto overflow = _numCellsY % _numThreads;
  auto startY = 0ul;
  auto endY = 0ul;
  for (int i = 0; i < _numThreads; i++)
  {
    endY += _rowsPerThread;
    if (overflow > 0)
    {
      overflow--;
      endY++;
    }
    _threadParams.emplace_back(startY, endY);
    startY = endY;
  }

  _inactiveColor = sf::Color(1, 7, 22);
  _activeColor = sf::Color(46, 137, 255);
}

GameOfLife::~GameOfLife()
{
}

void GameOfLife::onStart()
{
  Events::Game->bind(EGameEvent::CALC_NEIGHBORS_COMPLETE, this, &GameOfLife::calcNeighborComplete);
  Events::Game->bind(EGameEvent::ACTIVATE_CELLS_COMPLETE, this, &GameOfLife::setCellsComplete);

  _bIsPaused = true;
  _bPauseKey = false;
  _startDelaySec = 2.0f;
  _bStartDelayComplete = false;
  _bThreadsStarted = false;

  _cellGrid.setCellColor(_inactiveColor);
  _cellGrid.update();
  _numCells = _cellGrid.getWidth() * _cellGrid.getHeight();

  if (_activeCells == nullptr)
    _activeCells = std::make_shared<bool[]>(_numCells);
  if (_cellNeighbors == nullptr)
    _cellNeighbors = std::make_shared<uint8_t[]>(_numCells);

  std::fill(_activeCells.get(), _activeCells.get() + _numCells, false);
  std::fill(_cellNeighbors.get(), _cellNeighbors.get() + _numCells, 0);

  seedFromConfig("glider_gun", -20);
  startThreads();
}

sf::Vector2f GameOfLife::onResize(int screenX, int screenY)
{
  float minScreen = std::min(screenX, screenY);
  float minCells = std::min(_numCellsX, _numCellsY);

  _cellGrid.onResize(_numCellsX, _numCellsY, minScreen / minCells, 1.0f);
  return sf::Vector2f(std::max(screenX - screenY, 0), std::max(screenY - screenX, 0)) * 0.5f;
}

void GameOfLife::onEnd()
{
  Events::Game->unsubscribe(this);
  stopThreads();
}

void GameOfLife::processEvents(sf::Event& event)
{
  bool pauseKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
  if (pauseKey)
  {
    if (!_bPauseKey)
    {
      _bIsPaused = !_bIsPaused;
      Events::Game->publish(EGameEvent::TOGGLE_PAUSE, _bIsPaused);
    }
  }
  _bPauseKey = pauseKey;

  if (_bIsPaused)
  {
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        int x = (event.mouseButton.x - 1) / _cellGrid.getCellSpacing();
        int y = (event.mouseButton.y - 1) / _cellGrid.getCellSpacing();
        setCell(x, y, !getCell(x, y));
        _cellGrid.update();
      }
    }
  }
}

void GameOfLife::update(float ds)
{
  _loopTimer.stop();
  _lastLoopTime = _loopTimer.getMicroSecDuration();
  _loopTimer.start();

  for (auto& thread : _threads)
  {
    thread->setTargetLoopTime(_lastLoopTime);
  }

  if (!_bStartDelayComplete)
  {
    _startDelaySec -= ds;
    _bStartDelayComplete = _startDelaySec < 0.0f;
    if (_bStartDelayComplete && _bIsPaused)
    {
      _bIsPaused = false;
      Events::Game->publish(EGameEvent::TOGGLE_PAUSE, _bIsPaused);
    }
  }
}

void GameOfLife::render(sf::RenderWindow& window)
{
  window.draw(_cellGrid);
  // uncomment to test screen tearing
  // if (_cellGridLock.try_lock())
  // {
  //   window.draw(_cellGrid);
  //   _cellGridLock.unlock();
  // }
  // else
  // {
  //   Log().debug("lock failed");
  // }
}

void GameOfLife::startThreads()
{
  _numCalcNeighborComplete = 0;
  _numSetCellsComplete = 0;
  _bThreadsStarted = true;
  _bShutdownThreads = false;
  _lastLoopTime = std::chrono::milliseconds(10); // 100 ups
  _threads.clear();
  for (const auto& yRange : _threadParams)
  {
    _threads.emplace_back(std::make_unique<GameOfLifeWorker>());
    _threads.back()->init(yRange, _cellGrid.getWidth(), _cellGrid.getHeight(),
      _activeCells, _cellNeighbors);
    _threads.back()->start();
  }
  Events::Game->publish(EGameEvent::TOGGLE_PAUSE, _bIsPaused);
  Events::Game->publish(EGameEvent::CALC_NEIGHBORS);
}

void GameOfLife::stopThreads()
{
  _bShutdownThreads = true;
  if (_bThreadsStarted)
  {
    _threads.clear();
  }
  _bThreadsStarted = false;
}

void GameOfLife::calcNeighborComplete()
{
  if (++_numCalcNeighborComplete == _numThreads)
  {
    _numCalcNeighborComplete = 0;
    Events::Game->publish(EGameEvent::ACTIVATE_CELLS);
  }
}

void GameOfLife::setCellsComplete()
{
  if (++_numSetCellsComplete == _numThreads)
  {
    _numSetCellsComplete = 0;
    updateCellGrid();
    Events::Game->publish(EGameEvent::CALC_NEIGHBORS);
  }
}

void GameOfLife::updateCellGrid()
{
  _cellGridLock.lock();
  for (int i = 0; i < _numCells; i++)
  {
    _cellGrid.setCellColor(i, _activeCells[i] ? _activeColor : _inactiveColor);
  }
  _cellGrid.update();
  _cellGridLock.unlock();
}

void GameOfLife::basicSeed()
{
  int center_x = _cellGrid.getWidth() / 2;
  int center_y = _cellGrid.getHeight() / 2;

  // trash
  setCell(center_x - 4, center_y - 2, true);
  setCell(center_x - 3, center_y - 2, true);
  setCell(center_x + 4, center_y - 2, true);
  setCell(center_x + 5, center_y - 2, true);

  setCell(center_x - 4, center_y + 2, true);
  setCell(center_x - 3, center_y + 2, true);
  setCell(center_x + 4, center_y + 2, true);
  setCell(center_x + 5, center_y + 2, true);

  for (int y = -1; y < 2; y++)
  {
    setCell(center_x - 5, center_y - y, true);
    setCell(center_x - 2, center_y - y, true);
    setCell(center_x + 3, center_y - y, true);
    setCell(center_x + 6, center_y - y, true);
  }
}

bool GameOfLife::seedFromConfig(std::string configName, int offsetX, int offsetY)
{
  std::string yamlFileName = _resourcesPath + configName + ".yaml";

  YAML::Node config = YAML::LoadFile(yamlFileName);
  if (!config["imageName"] || !config["cellSize"] || !config["cellAliveColor"] || !config["cellDeadColor"])
  {
    Log().error("Could not load " + yamlFileName);
    return false;
  }

  std::string imageName = _resourcesPath + config["imageName"].as<std::string>();
  int cellSize = config["cellSize"].as<int>(10);
  sf::Color cellAliveColor = config["cellAliveColor"].as<sf::Color>();
  sf::Color cellDeadColor = config["cellDeadColor"].as<sf::Color>();

  sf::Image image;
  if (!image.loadFromFile(imageName))
  {
    Log().error("Could not load " + imageName);
    return false;
  }

  std::vector<bool> seed;
  int halfCellSize = cellSize / 2;
  int width = 0;
  int height = 0;
  for (int y = halfCellSize; y < image.getSize().y; y += cellSize)
  {
    for (int x = halfCellSize; x < image.getSize().x; x += cellSize)
    {
      // read dead cell color from image for corruption detection?
      seed.push_back(image.getPixel(x, y) == cellAliveColor);
      // only on first pass
      if (y == halfCellSize)
        width++;
    }
    height++;
  }

  // saveSeed(seed, width, height, "5.png");
  setSeed(seed, width, height, offsetX + _cellGrid.getWidth() / 2, offsetY + _cellGrid.getHeight() / 2);

  return true;
}

void GameOfLife::setSeed(const std::vector<bool>& seed, int width, int height, int center_x, int center_y)
{
  auto i = 0ul;
  int anchor_x = center_x - (width / 2);
  int anchor_y = center_y - (height / 2);
  for (auto y = 0ul; y < height; y++)
  {
    for (auto x = 0ul; x < width; x++)
    {
      if (seed[i++])
      {
        setCell(anchor_x + x, anchor_y + y, true);
      }
    }
  }
}

void GameOfLife::saveSeed(const std::vector<bool>& seed, int width, int height, std::string fileName)
{
  sf::Image image;
  image.create(width, height);
  int i = 0;
  for (auto y = 0ul; y < height; y++)
  {
    for (auto x = 0ul; x < width; x++)
    {
      if (seed[i++])
      {
        image.setPixel(x, y, sf::Color::White);
      }
    }
  }
  image.saveToFile(fileName);
}

bool GameOfLife::getCell(int x, int y) const
{
  return _activeCells[_cellGrid.getCellIndex(x, y)];
}

void GameOfLife::setCell(int i, bool alive)
{
  _activeCells[i] = alive;
  sf::Color color = _inactiveColor;
  if (alive)
  {
    color = _activeColor;
  }
  _cellGrid.setCellColor(i, color);
}

void GameOfLife::setCell(int x, int y, bool alive)
{
  setCell(_cellGrid.getCellIndex(x, y), alive);
}
