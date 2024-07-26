#include "PathGraph.hpp"
#include "Utils/Logging.hpp"
#include "Utils/VectorUtils.hpp"
#include "GameMode/ConsoleEvents.hpp"

PathGraph::PathGraph()
{
  _wallThickness = 1.0f;
  _cellSize = 10.0f;
  _hoverIndex = 0;
}

PathGraph::~PathGraph()
{

}

void PathGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for (const auto& debugLine : _pathGraphStateUI)
  {
    target.draw(debugLine, states);
  }
}

void PathGraph::setup(sf::Vector2u screenSize, uint32_t cellSize, uint32_t wallThickness)
{
  _screenSize = screenSize;
  _wallThickness = wallThickness;
  _cellSize = cellSize;
  _pathGraphSize = sf::IntRect(0, 0, _screenSize.x / _cellSize,  _screenSize.y / _cellSize);

  auto prefab = sf::RectangleShape(sf::Vector2f(_wallThickness * 2.0f, _wallThickness * 2.0f));
  prefab.setOrigin(_wallThickness, _wallThickness);
  _defaultColor = sf::Color(200, 200, 200);
  _hoverColor = sf::Color(100, 100, 100);
  prefab.setFillColor(stateColor(PathGraphState::PASSABLE));
  prefab.setOutlineColor(sf::Color(200, 200, 200));
  prefab.setOutlineThickness(0.0f);

  _pathGraphStateUI.clear();
  _cellState.clear();
  for (int y = 0; y < _pathGraphSize.height; y++)
  {
    for (int x = 0; x < _pathGraphSize.width; x++)
    {
      prefab.setPosition(sf::Vector2f(x * _cellSize + _cellSize * 0.5f, y * _cellSize + _cellSize * 0.5f));
      _pathGraphStateUI.push_back(prefab);
      _cellState.push_back(PathGraphState::PASSABLE);
    }
  }

  for (int x = 0; x < _pathGraphSize.width; x++)
  {
    setCellState(x, PathGraphState::WALL);
    setCellState(x + (_cellState.size() - _pathGraphSize.width), PathGraphState::WALL);
  }

  for (int y = 0; y < _pathGraphSize.height; y++)
  {
    setCellState(y * _pathGraphSize.width, PathGraphState::WALL);
    setCellState(y * _pathGraphSize.width + (_pathGraphSize.width - 1), PathGraphState::WALL);
  }

  setCellState(0, _pathGraphSize.height / 2, PathGraphState::START);
  setCellState(_pathGraphSize.width - 1, _pathGraphSize.height / 2, PathGraphState::END);

  // save("template_graph.png");

  resetPathSets();
}

void PathGraph::hover(sf::Vector2f mousePos)
{
  sf::Vector2i coord = screenToCoord(mousePos);
  bool updateText = false;
  if (_pathGraphSize.contains(coord))
  {
    size_t newHoverIndex = coord.x + (coord.y * _pathGraphSize.width);
    if (newHoverIndex != _hoverIndex)
    {
      _pathGraphStateUI[_hoverIndex].setOutlineThickness(0.0f);
      _pathGraphStateUI[newHoverIndex].setOutlineThickness(_wallThickness);
      _hoverIndex = newHoverIndex;
      updateText = true;
    }
  }
  else
  {
    _pathGraphStateUI[_hoverIndex].setOutlineThickness(0.0f);
    updateText = true;
  }
  if (!updateText)
    return;
  if (_cellState[_hoverIndex] == PathGraphState::WALL)
    Events::Console->publish("show_info_text", std::string("wall"), mousePos);
  else
    Events::Console->publish("hide_info_text");
}

void PathGraph::setCellState(int index, PathGraphState state)
{
  _cellState[index] = state;
  _pathGraphStateUI[index].setFillColor(stateColor(state));
}

void PathGraph::setCellState(int x, int y, PathGraphState state)
{
  setCellState(x + y * _pathGraphSize.width, state);
}

bool PathGraph::save(std::string fileName)
{
  std::string filePath = Str::format("pathfinding/", fileName);
  sf::Image image;
  image.create(_pathGraphSize.width, _pathGraphSize.height);
  int i = 0;
  for (auto y = 0ul; y < _pathGraphSize.height; y++)
  {
    for (auto x = 0ul; x < _pathGraphSize.width; x++)
    {
      if (_cellState[i++] == PathGraphState::WALL)
      {
        image.setPixel(x, y, sf::Color::Red);
      }
    }
  }
  auto saved = image.saveToFile(filePath);
  if (saved)
    Log().info("image saved: {}", filePath);
  else
    Log().info("could not save: {}", filePath);
  return saved;
}

bool PathGraph::load(std::string fileName)
{
  std::string filePath = Str::format("pathfinding/", fileName);
  sf::Image image;
  if (!image.loadFromFile(filePath))
  {
    Log().error("Could not load " + filePath);
    return false;
  }

  _cellState.clear();
  _cellState.reserve(image.getSize().x * image.getSize().y);
  _pathGraphSize = sf::IntRect(sf::Vector2i(), vu::cast<int>(image.getSize()));
  int i = 0;
  for (int y = 0; y < image.getSize().y; y++)
  {
    for (int x = 0; x < image.getSize().x; x++)
    {
      if (image.getPixel(x, y) == stateColor(PathGraphState::WALL))
        setCellState(i, PathGraphState::WALL);
      else if (image.getPixel(x, y) == stateColor(PathGraphState::PASSABLE))
        setCellState(i, PathGraphState::PASSABLE);
      else if (image.getPixel(x, y) == stateColor(PathGraphState::START))
        setCellState(i, PathGraphState::START);
      else if (image.getPixel(x, y) == stateColor(PathGraphState::END))
        setCellState(i, PathGraphState::END);
      i++;
    }
  }

  Log().info("image loaded: {}", filePath);
  return true;
}

sf::Vector2f PathGraph::getStart()
{
  for (int i = 0; i < _cellState.size(); i++)
  {
    if (_cellState[i] == PathGraphState::START)
    {
      return vu::cast<float>(indexToCoord(i)) * _cellSize;
    }
  }
  return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f PathGraph::getEnd()
{
  for (int i = 0; i < _cellState.size(); i++)
  {
    if (_cellState[i] == PathGraphState::END)
    {
      return vu::cast<float>(indexToCoord(i)) * _cellSize;
    }
  }
  return sf::Vector2f(0.0f, 0.0f);
}

sf::Color PathGraph::stateColor(PathGraphState state)
{
  if (state == PathGraphState::WALL)
  {
    return sf::Color(255, 100, 100);
  }
  else if (state == PathGraphState::PASSABLE)
  {
    return sf::Color(100, 100, 100);
  }
  else if (state == PathGraphState::START)
  {
    return sf::Color(100, 255, 100);
  }
  else if (state == PathGraphState::END)
  {
    return sf::Color(100, 100, 255);
  }

  return sf::Color(255, 0, 255);
}

sf::Vector2i PathGraph::indexToCoord(int index)
{
  return sf::Vector2i(index % _pathGraphSize.height, index / _pathGraphSize.width);
}

sf::Vector2i PathGraph::screenToCoord(sf::Vector2f screenPos)
{
  return sf::Vector2i(screenPos.x / _cellSize, screenPos.y / _cellSize);
}

sf::Vector2f PathGraph::coordToScreen(sf::Vector2i coord)
{
  return sf::Vector2f(coord.x * _cellSize, coord.y * _cellSize) + sf::Vector2f(_cellSize * 0.5f, _cellSize * 0.5f);
}

void PathGraph::resetPathSets()
{
  _openSet.clear();
  _closedSet.clear();
  int i = 0;
  for (int y = 0; y < _pathGraphSize.height; y++)
  {
    for (int x = 0; x < _pathGraphSize.width; x++)
    {
      _openSet[nodeHash(x, y)] = _cellState[i++];
    }
  }
}

uint64_t PathGraph::nodeHash(int x, int y)
{
  return uint64_t(x) | uint64_t(y) << sizeof(int) * CHAR_BIT;
}
