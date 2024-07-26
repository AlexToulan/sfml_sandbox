#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map> 

enum class PathGraphState : int
{
  WALL = -1,
  PASSABLE = 0,
  START = 1,
  END = 2
};

class PathGraph : public sf::Drawable
{
public:
  PathGraph();
  ~PathGraph();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void setup(sf::Vector2u screenSize, uint32_t cellSize = 10, uint32_t wallThickness = 1);
  void hover(sf::Vector2f mousePos);
  void setCellState(int index, PathGraphState state);
  void setCellState(int x, int y, PathGraphState state);
  bool save(std::string fileName);
  bool load(std::string fileName);
  void addObstacle(std::vector<sf::Vector2i> points);
  sf::Vector2f getStart();
  sf::Vector2f getEnd();

private:
  sf::Color stateColor(PathGraphState state);
  sf::Vector2i indexToCoord(int index);
  sf::Vector2i screenToCoord(sf::Vector2f screenPos);
  sf::Vector2f coordToScreen(sf::Vector2i coord);
  void resetPathSets();
  uint64_t nodeHash(int x, int y);

  std::vector<PathGraphState> _cellState;
  sf::IntRect _pathGraphSize;
  std::unordered_map<uint64_t, PathGraphState> _openSet;
  std::unordered_map<uint64_t, PathGraphState> _closedSet;

  sf::Vector2u _screenSize;
  float _wallThickness;
  float _cellSize;

  // UI
  std::vector<sf::RectangleShape> _pathGraphStateUI;
  size_t _hoverIndex;
  sf::Color _defaultColor;
  sf::Color _hoverColor;
};
