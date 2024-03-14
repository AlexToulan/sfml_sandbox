#include "GameOfLife/GameOfLife.hpp"
#include "GameMode/GameEvents.hpp"
#include "Utils/Logging.hpp"
#include "Utils/MathUtils.hpp"
#include "Utils/Yaml.hpp"

#include <source_location>

GameOfLife::GameOfLife()
 : EventListener()
 , GameMode(std::source_location::current().file_name())
{
}

GameOfLife::~GameOfLife()
{
}

void GameOfLife::onStart()
{
  _bIsPaused = true;
  _bPauseKey = false;
  _bLockToFrameRate = true;
  _startDelaySec = 1.0f;
  _bStartDelayComplete = false;
  _bStepKeyPressed = false;

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

  // _cellGrid.setup(100, 100, 10, 1, inactive);
  _cellGrid.setup(200, 200, 5, 1, inactive);

  _numCells = _cellGrid.getWidth() * _cellGrid.getHeight();
  // _activeCells = std::make_shared<bool[]>(_numCells);
  // _cellNeighbors = std::make_shared<int[]>(_numCells);
  _activeCells = new bool[_numCells] { false };
  _cellNeighbors = new int[_numCells] { 0 };

  _rowsProcessed = _cellGrid.getHeight();
  GameEvents.bind(EGameEvent::ACTIVATE_CELLS_COMPLETE, this, &GameOfLife::activateCellsComplete);
  GameEvents.bind(EGameEvent::CALC_NEIGHBORS_COMPLETE, this, &GameOfLife::calcNeighborsComplete);
  Log::info(std::to_string(_rowsProcessed));

  // don't start workers till after we stop changing cells
  // basicSeed();
  // seedFromConfig("pulsar");
  seedFromConfig("glider_gun");
  startWorkers(_cellGrid.getWidth(), _cellGrid.getHeight());
}

void GameOfLife::onEnd()
{
  GameEvents.unsubscribe(this);
  for (size_t i = 0; i < _workers.size(); i++)
  {
    _workers[i]->stop();
  }
  delete[] _activeCells;
  delete[] _cellNeighbors;
  _workers.clear();
}

void GameOfLife::processEvents(sf::Event& event)
{
  bool pauseKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
  if (pauseKey)
  {
    if (!_bPauseKey)
    {
      _bIsPaused = !_bIsPaused;
      if (!_bIsPaused)
        tryCalcNeighbors();
    }
  }
  _bPauseKey = pauseKey;
  
  // TODO: fix crash bug cause by mouse leaving render window
  if (_bIsPaused)
  {
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        Log::info(std::to_string(event.mouseButton.x) + " " + std::to_string(event.mouseButton.y));
        int x = (event.mouseButton.x - 1) / _cellGrid.getCellSpacing();
        int y = (event.mouseButton.y - 1) / _cellGrid.getCellSpacing();
        setCell(x, y, !getCell(x, y));
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !_bStepKeyPressed)
    {
      tryCalcNeighbors();
    }
    _bStepKeyPressed = event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
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

  if (_bIsPaused)
  {
    return;
  }
  if (_bLockToFrameRate)
  {
    tryCalcNeighbors();
  }
}

void GameOfLife::render(sf::RenderWindow& window)
{
  // update colors
  sf::Color color = _swatch[0];
  for (size_t i = 0; i < _numCells; i++)
  {
    color = _activeCells[i] ? _swatch[8] : _swatch[0];
    _cellGrid.setCellColor(i, color);
  }

  _cellGrid.update();
  window.draw(_cellGrid);
}

void GameOfLife::activateCellsComplete(const std::pair<int, int>& range)
{
  _rowsProcessed += range.second - range.first;
  Log::info(std::to_string(_rowsProcessed));
  if (!_bIsPaused && !_bLockToFrameRate)
  {
    tryCalcNeighbors();
  }
}

void GameOfLife::calcNeighborsComplete(const std::pair<int, int>& range)
{
  _rowsProcessed += range.second - range.first;
  Log::info(std::to_string(_rowsProcessed));
  if (_rowsProcessed == _cellGrid.getHeight())
  {
    _rowsProcessed = 0;
    GameEvents.publish(EGameEvent::ACTIVATE_CELLS);
  }
}

void GameOfLife::startWorkers(int width, int height)
{
  int numWorkers = 10;
  int yStride = _cellGrid.getHeight() / numWorkers;
  for (size_t i = 0; i < numWorkers; i++)
  {
    _workers.push_back(std::unique_ptr<GameOfLifeWorker>(new GameOfLifeWorker()));
    _workers[i]->init(0, _cellGrid.getWidth(), yStride * i, yStride * (i + 1), width, height,
      _activeCells, _cellNeighbors); // yuck
    _workers[i]->start();
  }

  if (!_bIsPaused)
  {
    GameEvents.publish(EGameEvent::CALC_NEIGHBORS);
  }
}

bool GameOfLife::tryCalcNeighbors()
{
  if (_rowsProcessed == _cellGrid.getHeight())
  {
    _rowsProcessed = 0;
    GameEvents.publish(EGameEvent::CALC_NEIGHBORS);
    return true;
  }
  Log::warn("couldn't restart game of life");
  return false;
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

bool GameOfLife::seedFromConfig(std::string configName)
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

  saveSeed(seed, width, height, "5.png");
  setSeed(seed, width, height, _cellGrid.getWidth() / 2, _cellGrid.getHeight() / 2);

  return true;
}

void GameOfLife::setSeed(const std::vector<bool>& seed, int width, int height, int center_x, int center_y)
{
  int i = 0;
  int anchor_x = center_x - (width / 2);
  int anchor_y = center_y - (height / 2);
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
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
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      if (seed[i++])
      {
        image.setPixel(x, y, sf::Color::White);
      }
    }
  }
  image.saveToFile(fileName);
}

bool GameOfLife::getCell(int x, int y)
{
  return _activeCells[_cellGrid.getCellIndex(x, y)];
}

void GameOfLife::setCell(int x, int y, bool alive)
{
  size_t index = _cellGrid.getCellIndex(x, y);
  _activeCells[index] = alive;
  sf::Color color = _swatch[0];
  int neighborDelta = 0;
  if (alive)
  {
    color = _swatch[8];
    neighborDelta = 1;
  }
  _cellGrid.setCellColor(x, y, color);
}
