#include "GameOfLife/GameOfLife.hpp"
#include "GameMode/GameEvents.hpp"
#include "Utils/Logging.hpp"
#include "Utils/MathUtils.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Yaml.hpp"

#include <algorithm>
#include <shared_mutex>
#include <source_location>

GameOfLife::GameOfLife()
 : EventListener()
 , GameMode(std::source_location::current().file_name())
 , _activeCells(nullptr)
 , _cellNeighbors(nullptr)
{
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
}

GameOfLife::~GameOfLife()
{
}

void GameOfLife::onStart()
{
  _bIsPaused = true;
  _bPauseKey = false;
  _startDelaySec = 2.0f;
  _bStartDelayComplete = false;
  _bStepKeyPressed = false;
  _bThreadsStarted = false;

  const sf::Color inactive = sf::Color(1, 7, 22);
  _swatch = std::unique_ptr<sf::Color[]>(new sf::Color[9]
  {
    inactive,
    sf::Color(2, 20, 46),
    sf::Color(6, 34, 72),
    sf::Color(11, 49, 99),
    sf::Color(18, 66, 128),
    sf::Color(24, 83, 158),
    sf::Color(31, 100, 190),
    sf::Color(39, 118, 222),
    sf::Color(46, 137, 255),
  });
  _cellGrid.setCellColor(inactive);
  _cellGrid.update();
  _numCells = _cellGrid.getWidth() * _cellGrid.getHeight();

  if (_activeCells == nullptr)
    _activeCells = std::make_unique<bool[]>(_numCells);
  if (_cellNeighbors == nullptr)
    _cellNeighbors = std::make_unique<uint8_t[]>(_numCells);

  std::fill(_activeCells.get(), _activeCells.get() + _numCells, false);
  std::fill(_cellNeighbors.get(), _cellNeighbors.get() + _numCells, 0);

  // basicSeed();
  // seedFromConfig("pulsar");
  seedFromConfig("glider_gun", -20);
  // run once to init
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
      }
    }
    bool bEnter = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
    if (bEnter && !_bStepKeyPressed)
    {
      // restartWorkers();
    }
    _bStepKeyPressed = bEnter;
  }
}

void GameOfLife::update(float ds)
{
  if (!_bStartDelayComplete)
  {
    _startDelaySec -= ds;
    _bStartDelayComplete = _startDelaySec < 0.0f;
    if (_bStartDelayComplete && _bIsPaused)
    {
      _bIsPaused = false;
    }
  }

  _loopTimer.stop();
  _lastLoopTime = _loopTimer.getMicroSecDuration();
  _loopTimer.start();
  if (_threadsSettingCells == 0)
  {
    _cellGrid.update();
  }
}

void GameOfLife::render(sf::RenderWindow& window)
{
  window.draw(_cellGrid);
}

void GameOfLife::startThreads()
{
  _bThreadsStarted = true;
  _bShutdownThreads = false;
  _calcNeighborComplete = false;
  _setCellsComplete = false;

  _threadsCalcNeighbors = 0;
  _threadsSettingCells = 0;
  _threadsWorking = _numThreads;
  _lastLoopTime = std::chrono::milliseconds(100);
  _threads.clear();
  _threadTimers.clear();
  _threadSync = std::thread(&GameOfLife::threadSync, this);
  for (const auto& pair : _threadParams)
  {
    _threads.emplace_back(&GameOfLife::classicRules, this, pair.first, pair.second);
    _threadTimers[pair.first] = Timer();
  }
}

void GameOfLife::stopThreads()
{
  _bShutdownThreads = true;
  if (_bThreadsStarted)
  {
    for (auto& thread : _threads)
    {
      thread.join();
    }
  }
  _threadSync.join();
  _bThreadsStarted = false;
}

void GameOfLife::classicRules(int startY, int endY)
{
  while (!_bShutdownThreads)
  {
    if (_bIsPaused)
    {
      _threadsWorking--;
      std::this_thread::sleep_for(_lastLoopTime);
      _threadsWorking++;
      continue;
    }

    _threadTimers[startY].start();
    {
      calcNumNeighborsAlive(startY, endY);
      {
        std::unique_lock<std::mutex> un(cnMut);
        _threadsCalcNeighbors++;
        cnCv.wait(un, [this] { return _calcNeighborComplete; });
      }
      setCellsClassicRules(startY, endY);
      {
        std::unique_lock<std::mutex> un(scMut);
        _threadsSettingCells++;
        scCv.wait(un, [this] { return _setCellsComplete; });
      }
    }
    _threadTimers[startY].stop();

    _threadsWorking--;
    std::this_thread::sleep_for(_lastLoopTime - _threadTimers[startY].getMicroSecDuration());
    _threadsWorking++;
  }
}

void GameOfLife::threadSync()
{
  while (!_bShutdownThreads)
  {
    if (_threadsCalcNeighbors == _numThreads)
    {
      Log::debug("cn");
      cnCv.notify_all();
      std::lock_guard<std::mutex> un(cnMut);
      _calcNeighborComplete = true;
      _setCellsComplete = false;
      _threadsCalcNeighbors = 0;
    }
    if (_threadsSettingCells == _numThreads)
    {
      Log::debug("sc");
      scCv.notify_all();
      std::lock_guard<std::mutex> un(scMut);
      _calcNeighborComplete = false;
      _setCellsComplete = true;
      _threadsSettingCells = 0;
    }
  }
}

void GameOfLife::setCellsClassicRules(int startY, int endY)
{
  for (auto i = startY * _numCellsX; i < endY * _numCellsX; i++)
  {
    if (_activeCells[i])
    {
      if (_cellNeighbors[i] < 2)
        setCell(i, false);
      if (_cellNeighbors[i] > 3)
        setCell(i, false);
    }
    else
    {
      if (_cellNeighbors[i] == 3)
        setCell(i, true);
    }
  }
}

void GameOfLife::calcNumNeighborsAlive(int startY, int endY)
{
  for (int y = startY; y < endY; y++)
  {
    for (int x = 0; x < _numCellsX; x++)
    {
      _cellNeighbors[_cellGrid.getCellIndex(x, y)] = getNumNeighborsAlive(x, y);
    }
  }
}

int GameOfLife::getNumNeighborsAlive(int x, int y)
{
  // wrap field!
  int numNeighborsAlive = 0;
  for (int ny = y - 1; ny <= y + 1; ny++)
  {
    for (int nx = x - 1; nx <= x + 1; nx++)
    {
      if (nx == x && ny == y)
        continue;
      if (getCell(mu::wrap(nx, 0, (int)_numCellsX - 1), mu::wrap(ny, 0, (int)_numCellsY - 1)))
        numNeighborsAlive++;
    }
  }
  return numNeighborsAlive;
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
    Log::error("Could not load " + yamlFileName);
    return false;
  }

  std::string imageName = _resourcesPath + config["imageName"].as<std::string>();
  int cellSize = config["cellSize"].as<int>(10);
  sf::Color cellAliveColor = config["cellAliveColor"].as<sf::Color>();
  sf::Color cellDeadColor = config["cellDeadColor"].as<sf::Color>();

  sf::Image image;
  if (!image.loadFromFile(imageName))
  {
    Log::error("Could not load " + imageName);
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
      // add dead cell color for corruption detection?
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
  sf::Color color = _swatch[0];
  if (alive)
  {
    color = _swatch[8];
  }
  _cellGrid.setCellColor(i, color);
}

void GameOfLife::setCell(int x, int y, bool alive)
{
  setCell(_cellGrid.getCellIndex(x, y), alive);
}
