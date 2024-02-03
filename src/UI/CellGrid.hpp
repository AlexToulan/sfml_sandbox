#pragma once

#include <SFML/Graphics.hpp>

class CellGrid : public sf::Drawable
{
public:
  CellGrid();
  ~CellGrid();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void update();
  void setup(int width = 10, int height = 10, int cellSpacing = 10, int padding = 0, sf::Color color = sf::Color::Black);
  void setCellColor(sf::Color = sf::Color::Black);
  void setCellColor(int i, sf::Color = sf::Color::Black);
  void setCellColor(int x, int y, sf::Color = sf::Color::Black);

  int getWidth() const;
  int getHeight() const;
  int getCellSpacing() const;
  size_t getCellIndex(int x, int y) const;

private:
  int _width;
  int _height;
  int _cellSpacing;
  int _padding;
  int _cellSize;
  int _vertsPerQuat;

  std::unique_ptr<sf::Vertex[]> _verts;
  sf::VertexBuffer _cells;
};