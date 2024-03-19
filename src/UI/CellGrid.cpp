#include "UI/CellGrid.hpp"
#include "Utils/MathUtils.hpp"

CellGrid::CellGrid()
 : _verts(nullptr)
 , _cells(sf::PrimitiveType::Quads, sf::VertexBuffer::Usage::Stream)
{

}

CellGrid::~CellGrid()
{
}

void CellGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(_cells, states);
}

void CellGrid::update()
{
  _cells.update(_verts.get());
}

void CellGrid::onResize(int width, int height, float cellSpacing, float padding)
{
  _width = std::max(width, 1);
  _height = std::max(height, 1);
  _numCells = _width * _height;

  _cellSpacing = std::max(cellSpacing, 1.0f);
  _padding = padding;
  _cellSize = _cellSpacing - _padding * 2; // padding on all sides
  _vertsPerQuat = 4;
  int numQuads = _numCells * _vertsPerQuat;

  if (_cells.getVertexCount() != numQuads)
  {
    _cells.create(numQuads);
    _verts = std::unique_ptr<sf::Vertex[]>(new sf::Vertex[numQuads]());
  }

  for(int y = 0; y < _height; y++)
  {
    for(int x = 0; x < _width; x++) // use x for the inner loop for cache performance
    {
      int quadPos = getCellIndex(x, y) * _vertsPerQuat;
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding,             (float)y * _cellSpacing + _padding);
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding + _cellSize, (float)y * _cellSpacing + _padding);
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding + _cellSize, (float)y * _cellSpacing + _padding + _cellSize);
      _verts[quadPos++].position = sf::Vector2((float)x * _cellSpacing + _padding,             (float)y * _cellSpacing + _padding + _cellSize);
    }
  }
  update();
}

void CellGrid::setCellColor(sf::Color color)
{
  for(int i = 0; i < _numCells; i++)
  {
    setCellColor(i, color);
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

size_t CellGrid::getCellIndexWrapped(int x, int y) const
{
  x = mu::wrap(x, 0, _width);
  y = mu::wrap(y, 0, _height);
  return y * _width + x;
}