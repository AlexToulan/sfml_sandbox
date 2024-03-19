#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class CellGrid : public sf::Drawable
{
public:
  CellGrid();
  ~CellGrid();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void update();
  void onResize(int width = 10, int height = 10, float cellSpacing = 10, float padding = 0);
  void setCellColor(sf::Color = sf::Color::Black);
  void setCellColor(int i, sf::Color = sf::Color::Black);
  void setCellColor(int x, int y, sf::Color = sf::Color::Black);

  int getWidth() const;
  int getHeight() const;
  int getCellSpacing() const;
  size_t getCellIndex(int x, int y) const;
  size_t getCellIndexWrapped(int x, int y) const;

private:
  int _width;
  int _height;
  int _numCells;
  float _cellSpacing;
  float _padding;
  float _cellSize;
  int _vertsPerQuat;

  std::unique_ptr<sf::Vertex[]> _verts;
  sf::VertexBuffer _cells;
};