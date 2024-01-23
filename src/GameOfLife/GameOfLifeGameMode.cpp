#include "GameOfLife/GameOfLifeGameMode.hpp"
#include "Utils/Logging.hpp"

#include <source_location>

GameOfLifeGameMode::GameOfLifeGameMode()
 : GameMode(std::source_location::current().file_name())
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
  _swatch = new sf::Color[9]
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
  };

  _cellGrid.setup(100, 100, 10, 1, inactive);

  int numCells = _cellGrid.getWidth() * _cellGrid.getHeight();
  _activeCells.reserve(numCells);
  _cellNeighbors.reserve(numCells);

  for (int i = 0; i < numCells; i++)
  {
    _activeCells.push_back(false);
    _cellNeighbors.push_back(0);
  }

  basicSeed();
  _secPerUpdate = 0.2;
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

void GameOfLifeGameMode::update(float delta_seconds)
{
  if (_bIsPaused)
  {
    return;
  }

  _currentUpdateSec += delta_seconds;
  if (_currentUpdateSec < _secPerUpdate)
    return;

  _currentUpdateSec -= _secPerUpdate;

  int i = 0;
  for (int y = 0; y < _cellGrid.getHeight(); y++)
  {
    for (int x = 0; x < _cellGrid.getWidth(); x++)
    {
      _cellNeighbors[i++] = calcNumNeighborsAlive(x, y);
    }
  }

  for (int y = 0; y < _cellGrid.getHeight(); y++)
  {
    for (int x = 0; x < _cellGrid.getWidth(); x++)
    {
      int cellIndex = _cellGrid.getCellIndex(x, y);
      if (_activeCells[cellIndex])
      {
        if (_cellNeighbors[cellIndex] < 2)
          setCell(x, y, false);
        if (_cellNeighbors[cellIndex] > 3)
          setCell(x, y, false);
      }
      else
      {
        if (_cellNeighbors[cellIndex] == 3)
          setCell(x, y, true);
      }
    }
  }
}

void GameOfLifeGameMode::render(sf::RenderWindow& window)
{
  // update colors
  sf::Color color = _swatch[0];
  for (size_t i = 0; i < _activeCells.size(); i++)
  {
    color = _activeCells[i] ? _swatch[8] : _swatch[0];
    _cellGrid.setCellColor(i, color);
  }
  _cellGrid.update();
  window.draw(_cellGrid);
}

void GameOfLifeGameMode::onEnd()
{
  // clearing and recreating this in onStart causes a crash. window likely holds a ref to VertexBuffer
  // _cells.create(0);
  _activeCells.clear();
  _cellNeighbors.clear();
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

  for (int y = - 1; y < 2; y++)
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

int GameOfLifeGameMode::wrap(int value, int min, int max)
{
  if (value < min)
    return max;
  if (value > max)
    return min;
  return value;
}

int GameOfLifeGameMode::calcNumNeighborsAlive(int x, int y)
{
  // wrap field!
  int numNeighborsAlive = 0;
  for (int ny = y - 1; ny <= y + 1; ny++)
  {
    for (int nx = x - 1; nx <= x + 1; nx++)
    {
      if (nx == x && ny == y)
        continue;
      if (getCell(wrap(nx, 0, _cellGrid.getWidth() - 1), wrap(ny, 0, _cellGrid.getHeight() - 1)))
       numNeighborsAlive++;
    }
  }
  return numNeighborsAlive;
}