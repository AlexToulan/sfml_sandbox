#include "Utils/UI/CellGrid.hpp"

CellGrid::CellGrid()
 : _verts(nullptr)
{

}

CellGrid::~CellGrid()
{
  delete[] _verts;
}

void CellGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(_cells, states);
}

void CellGrid::update()
{
  _cells.update(_verts);
}

void CellGrid::setup(int width, int height, int cellSpacing, int padding, sf::Color color)
{
  _width = width;
  _height = height;
  _cellSpacing = cellSpacing;
  _padding = padding;
  _cellSize = _cellSpacing - _padding * 2; // padding on all sides
  _vertsPerQuat = 4;
  int numCells = _width * _height;

  if (_cells.getVertexCount() == 0)
  {
    _cells = sf::VertexBuffer(sf::PrimitiveType::Quads, sf::VertexBuffer::Usage::Stream);
    _cells.create(numCells * _vertsPerQuat);
  }

  if (_verts == nullptr)
    _verts = new sf::Vertex[numCells * _vertsPerQuat]();

  for(int y = 0; y < _height; y++)
  {
    for(int x = 0; x < _width; x++) // use x for the inner loop for cache performance
    {
      int quadPos = getCellIndex(x, y) * _vertsPerQuat;
      int quadColor = quadPos;
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding,             (float)y * _cellSpacing + _padding);
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding + _cellSize, (float)y * _cellSpacing + _padding);
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding + _cellSize, (float)y * _cellSpacing + _padding + _cellSize);
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding,             (float)y * _cellSpacing + _padding + _cellSize);
      _verts[quadColor++].color = color;
      _verts[quadColor++].color = color;
      _verts[quadColor++].color = color;
      _verts[quadColor++].color = color;
    }
  }
  update();
}

void CellGrid::setCellColor(sf::Color color)
{
  for(int y = 0; y < _height; y++)
  {
    for(int x = 0; x < _width; x++)
    {
      setCellColor(color);
    }
  }
}

void CellGrid::setCellColor(int i, sf::Color color)
{
  int quadPos = i * _vertsPerQuat;
  _verts[quadPos++].color = color;
  _verts[quadPos++].color = color;
  _verts[quadPos++].color = color;
  _verts[quadPos++].color = color;
}

void CellGrid::setCellColor(int x, int y, sf::Color color)
{
  int quadPos = getCellIndex(x, y) * _vertsPerQuat;
  _verts[quadPos++].color = color;
  _verts[quadPos++].color = color;
  _verts[quadPos++].color = color;
  _verts[quadPos++].color = color;
}

int CellGrid::getWidth() const
{
  return _width;
}

int CellGrid::getHeight() const
{
  return _height;
}

int CellGrid::getCellSpacing() const
{
  return _cellSpacing;
}

size_t CellGrid::getCellIndex(int x, int y) const
{
  return y * _width + x;
}