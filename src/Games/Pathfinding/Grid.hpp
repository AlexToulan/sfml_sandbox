#pragma once

#include <SFML/Graphics.hpp>

enum class GridState : int
{
  WALL = -1,
  PASSABLE = 0,
  START = 1,
  END = 2
};

class Grid : public sf::Drawable
{
public:
  Grid();
  ~Grid();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void setup(sf::Vector2u screenSize, uint32_t cellSize = 10, uint32_t wallThickness = 1);
  void hover(sf::Vector2f mousePos);
  void setCellState(int index, GridState state);
  void setCellState(int x, int y, GridState state);
  bool save(std::string fileName);
  bool load(std::string fileName);

private:
  sf::Color stateColor(GridState state);
  sf::Vector2i screenToCoord(sf::Vector2f screenPos);
  sf::Vector2f coordToScreen(sf::Vector2i coord);

  std::vector<GridState> _cellState;
  sf::IntRect _gridSize;

  sf::Vector2u _screenSize;
  float _wallThickness;
  float _cellSize;

  // UI
  std::vector<sf::RectangleShape> _gridStateUI;
  size_t _hoverIndex;
  sf::Color _defaultColor;
  sf::Color _hoverColor;
};
