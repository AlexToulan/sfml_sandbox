#include "Grid.hpp"
#include "Utils/Logging.hpp"
#include "Utils/VectorUtils.hpp"
#include "GameMode/ConsoleEvents.hpp"

Grid::Grid()
{
  _wallThickness = 1.0f;
  _cellSize = 10.0f;
  _hoverIndex = 0;
}

Grid::~Grid()
{

}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for (const auto& debugLine : _gridStateUI)
  {
    target.draw(debugLine, states);
  }
}

void Grid::setup(sf::Vector2u screenSize, uint32_t cellSize, uint32_t wallThickness)
{
  _screenSize = screenSize;
  _wallThickness = wallThickness;
  _cellSize = cellSize;
  _gridSize = sf::IntRect(0, 0, _screenSize.x / _cellSize,  _screenSize.y / _cellSize);

  auto prefab = sf::RectangleShape(sf::Vector2f(_wallThickness * 2.0f, _wallThickness * 2.0f));
  prefab.setOrigin(_wallThickness, _wallThickness);
  _defaultColor = sf::Color(200, 200, 200);
  _hoverColor = sf::Color(100, 100, 100);
  prefab.setFillColor(stateColor(GridState::PASSABLE));
  prefab.setOutlineColor(sf::Color(200, 200, 200));
  prefab.setOutlineThickness(0.0f);

  _gridStateUI.clear();
  _cellState.clear();
  for (int y = 0; y < _gridSize.height; y++)
  {
    for (int x = 0; x < _gridSize.width; x++)
    {
      prefab.setPosition(sf::Vector2f(x * _cellSize + _cellSize * 0.5f, y * _cellSize + _cellSize * 0.5f));
      _gridStateUI.push_back(prefab);
      _cellState.push_back(GridState::PASSABLE);
    }
  }

  for (int x = 0; x < _gridSize.width; x++)
  {
    setCellState(x, GridState::WALL);
    setCellState(x + (_cellState.size() - _gridSize.width), GridState::WALL);
  }

  for (int y = 0; y < _gridSize.height; y++)
  {
    setCellState(y * _gridSize.width, GridState::WALL);
    setCellState(y * _gridSize.width + (_gridSize.width - 1), GridState::WALL);
  }
}

void Grid::hover(sf::Vector2f mousePos)
{
  sf::Vector2i coord = screenToCoord(mousePos);
  bool updateText = false;
  if (_gridSize.contains(coord))
  {
    size_t newHoverIndex = coord.x + (coord.y * _gridSize.width);
    if (newHoverIndex != _hoverIndex)
    {
      _gridStateUI[_hoverIndex].setOutlineThickness(0.0f);
      _gridStateUI[newHoverIndex].setOutlineThickness(_wallThickness);
      _hoverIndex = newHoverIndex;
      updateText = true;
    }
  }
  else
  {
    _gridStateUI[_hoverIndex].setOutlineThickness(0.0f);
    updateText = true;
  }
  if (!updateText)
    return;
  if (_cellState[_hoverIndex] == GridState::WALL)
    Events::Console->publish("show_info_text", std::string("wall"), mousePos);
  else
    Events::Console->publish("hide_info_text");
}

void Grid::setCellState(int index, GridState state)
{
  _cellState[index] = state;
  _gridStateUI[index].setFillColor(stateColor(state));
}

void Grid::setCellState(int x, int y, GridState state)
{
  setCellState(x + y * _gridSize.width, state);
}

bool Grid::save(std::string fileName)
{
  sf::Image image;
  image.create(_gridSize.width, _gridSize.height);
  int i = 0;
  for (auto y = 0ul; y < _gridSize.height; y++)
  {
    for (auto x = 0ul; x < _gridSize.width; x++)
    {
      if (_cellState[i++] == GridState::WALL)
      {
        image.setPixel(x, y, sf::Color::Red);
      }
    }
  }
  auto saved = image.saveToFile(fileName);
  if (saved)
    Log().info("image saved: {}", fileName);
  else
    Log().info("could not save: {}", fileName);
  return saved;
}

bool Grid::load(std::string fileName)
{
  sf::Image image;
  if (!image.loadFromFile(fileName))
  {
    Log().error("Could not load " + fileName);
    return false;
  }

  _cellState.clear();
  _cellState.reserve(image.getSize().x * image.getSize().y);
  _gridSize = sf::IntRect(sf::Vector2i(), vu::cast<int>(image.getSize()));
  int i = 0;
  for (int y = 0; y < image.getSize().y; y++)
  {
    for (int x = 0; x < image.getSize().x; x++)
    {
      if (image.getPixel(x, y) == stateColor(GridState::WALL))
        setCellState(i, GridState::WALL);
      else if (image.getPixel(x, y) == stateColor(GridState::PASSABLE))
        setCellState(i, GridState::PASSABLE);
      else if (image.getPixel(x, y) == stateColor(GridState::START))
        setCellState(i, GridState::START);
      else if (image.getPixel(x, y) == stateColor(GridState::END))
        setCellState(i, GridState::END);
      i++;
    }
  }

  Log().info("image loaded: {}", fileName);
  return true;
}

sf::Color Grid::stateColor(GridState state)
{
  if (state == GridState::WALL)
  {
    return sf::Color(255, 100, 100);
  }
  else if (state == GridState::PASSABLE)
  {
    return sf::Color(100, 100, 100);
  }
  else if (state == GridState::START)
  {
    return sf::Color(100, 255, 100);
  }
  else if (state == GridState::END)
  {
    return sf::Color(100, 100, 255);
  }

  return sf::Color(255, 0, 255);
}

sf::Vector2i Grid::screenToCoord(sf::Vector2f screenPos)
{
  return sf::Vector2i(screenPos.x / _cellSize, screenPos.y / _cellSize);
}

sf::Vector2f Grid::coordToScreen(sf::Vector2i coord)
{
  return sf::Vector2f(coord.x * _cellSize, coord.y * _cellSize) + sf::Vector2f(_cellSize * 0.5f, _cellSize * 0.5f);
}
