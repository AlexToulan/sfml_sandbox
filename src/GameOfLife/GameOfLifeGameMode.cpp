#include "EventTypes.hpp"
#include "GameOfLife/GameOfLifeGameMode.hpp"
#include "Utils/Logging.hpp"

#include <source_location>

GameOfLifeGameMode::GameOfLifeGameMode()
 : GameMode(std::source_location::current().file_name())
 , EventComponent()
{

}

GameOfLifeGameMode::~GameOfLifeGameMode()
{
}

void GameOfLifeGameMode::onStart()
{
  _bIsPaused = false;
  _bPauseKey = false;

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

  _cellGrid.setup(100, 100, 10, 1, inactive);

  _numCells = _cellGrid.getWidth() * _cellGrid.getHeight();
  _activeCells = std::make_shared<bool[]>(_numCells);
  _cellNeighbors = std::make_shared<int[]>(_numCells);

  _rowsProcessed = 0;
  subscribe(EventType::ACTIVATE_CELLS_COMPLETE, &GameOfLifeGameMode::activateCellsComplete);
  subscribe(EventType::CALC_NEIGHBORS_COMPLETE, &GameOfLifeGameMode::calcNeighborsComplete);

  // don't start workers till after we stop changing cells
  basicSeed();
  startWorkers(_cellGrid.getWidth(), _cellGrid.getHeight());
}

void GameOfLifeGameMode::onEnd()
{
  // clearing and recreating this in onStart causes a crash. window likely holds a ref to VertexBuffer
  // _cells.create(0);
  for (size_t i = 0; i < _workers.size(); i++)
  {
    _workers[i]->stop();
  }
  _workers.clear();
}

void GameOfLifeGameMode::processEvents(sf::Event& event)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
  {
    if (!_bPauseKey)
    {
      _bIsPaused = !_bIsPaused;
    }
    _bPauseKey = true;
  }
  else
  {
    _bPauseKey = false;
  }
  
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
  }
}

void GameOfLifeGameMode::update(float ds)
{
  if (_bIsPaused)
  {
    return;
  }
  // _cellMutex.lock();
}

void GameOfLifeGameMode::render(sf::RenderWindow& window)
{
  // update colors
  sf::Color color = _swatch[0];
  // _cellMutex.lock(); // only prevents screen tearing as it is only a read
  for (size_t i = 0; i < _numCells; i++)
  {
    color = _activeCells[i] ? _swatch[8] : _swatch[0];
    _cellGrid.setCellColor(i, color);
  }
  // _cellMutex.unlock();
  
  _cellGrid.update();
  window.draw(_cellGrid);
}

void GameOfLifeGameMode::activateCellsComplete(const EventBase& event)
{
  auto range = unpack<std::pair<int, int>>(event);
  _rowsProcessed += range.second - range.first;
  if (_rowsProcessed == _cellGrid.getHeight())
  {
    // _cellMutex.unlock();
    _rowsProcessed = 0;
    EventComponent::publish(EventType::CALC_NEIGHBORS);
  }
}

void GameOfLifeGameMode::calcNeighborsComplete(const EventBase& event)
{
  auto range = unpack<std::pair<int, int>>(event);
  _rowsProcessed += range.second - range.first;
  if (_rowsProcessed == _cellGrid.getHeight())
  {
    _rowsProcessed = 0;
    // _cellMutex.lock();
    EventComponent::publish(EventType::ACTIVATE_CELLS);
  }
}

void GameOfLifeGameMode::startWorkers(int width, int height)
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

  EventComponent::publish(EventType::CALC_NEIGHBORS);
}

void GameOfLifeGameMode::basicSeed()
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

bool GameOfLifeGameMode::getCell(int x, int y)
{
  return _activeCells[_cellGrid.getCellIndex(x, y)];
}

void GameOfLifeGameMode::setCell(int x, int y, bool alive)
{
  size_t index = _cellGrid.getCellIndex(x, y);
  _activeCells[index] = alive;
  sf::Color color = _swatch[0];
  if (alive)
  {
    color = _swatch[8];
  }
  _cellGrid.setCellColor(x, y, color);
}