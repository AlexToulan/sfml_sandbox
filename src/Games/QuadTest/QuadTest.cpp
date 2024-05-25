#include "QuadTest.hpp"
#include <source_location>

QuadTest::QuadTest(sf::Vector2u originalScreenSize)
 : GameMode(originalScreenSize)
 , _verts(nullptr)
{

}

QuadTest::~QuadTest()
{
  delete[] _verts;
}

void QuadTest::onStart()
{
  const int width = 100;
  const int height = 100;
  const int vertsPerQuat = 4;

  _width = width;
  _height = height;
  _cellSize = 10.0f;
  _padding = 0.0f;
  _vertsPerQuat = vertsPerQuat;

  const float cellSpacing = _cellSize + _padding * 2.0f;
  const std::size_t numCells = width * height;
  const sf::Color inactive = sf::Color(16, 16, 16);
  _swatch = std::unique_ptr<sf::Color[]> (new sf::Color[9]
  {
    sf::Color(255, 122, 213),
    sf::Color(199, 69, 255),
    sf::Color(97, 69, 255),
    sf::Color(69, 209, 255),
    sf::Color(69, 255, 119),
    sf::Color(252, 255, 69),
    sf::Color(255, 178, 69),
    sf::Color(255, 85, 69),
    inactive,
  });

  if (_cells.getVertexCount() == 0)
  {
    _cells = sf::VertexBuffer(sf::PrimitiveType::Quads, sf::VertexBuffer::Usage::Stream);
    _cells.create(numCells * _vertsPerQuat);
  }

  if (_verts == nullptr)
    _verts = new sf::Vertex[numCells * vertsPerQuat]();

  srand(0);

  std::random_device rd;  // a seed source for the random number engine
  _gen = std::mt19937(rd()); // mersenne_twister_engine seeded with rd()
  _distrib = std::uniform_int_distribution<>(1, _height / 2);

  for(int y = 0; y < _height; y++)
  {
    for(int x = 0; x < _width; x++)
    {
      int quadPos = (y * _width + x) * _vertsPerQuat;
      int quadColor = (y * _width + x) * _vertsPerQuat;
      _verts[quadPos++].position = sf::Vector2((float)x * cellSpacing + _padding,            (float)y * cellSpacing + _padding);
      _verts[quadPos++].position = sf::Vector2((float)x * cellSpacing + _padding + _cellSize, (float)y * cellSpacing + _padding);
      _verts[quadPos++].position = sf::Vector2((float)x * cellSpacing + _padding + _cellSize, (float)y * cellSpacing + _padding + _cellSize);
      _verts[quadPos++].position = sf::Vector2((float)x * cellSpacing + _padding,            (float)y * cellSpacing + _padding + _cellSize);
      _verts[quadColor++].color = inactive;
      _verts[quadColor++].color = inactive;
      _verts[quadColor++].color = inactive;
      _verts[quadColor++].color = inactive;
    }
  }
}

sf::Vector2f QuadTest::onResize(int screenX, int screenY)
{
  return sf::Vector2f();
}

void QuadTest::processEvents(sf::Event& event)
{

}

void QuadTest::update(float ds)
{
  for(int y = 0; y < _height; y++)
  {
    for(int x = 0; x < _width; x++)
    {
      int quadColor = (y * _width + x) * _vertsPerQuat;
      float dis_x = std::abs(x - _width / 2);
      float dis_y = std::abs(y - _height / 2);
      float dis = sqrt(dis_x * dis_x + dis_y * dis_y) / (float)_width;
      _verts[quadColor++].color = _swatch[(int)std::min(_distrib(_gen) * dis * 8, 8.0f)];
      _verts[quadColor++].color = _swatch[(int)std::min(_distrib(_gen) * dis * 8, 8.0f)];
      _verts[quadColor++].color = _swatch[(int)std::min(_distrib(_gen) * dis * 8, 8.0f)];
      _verts[quadColor++].color = _swatch[(int)std::min(_distrib(_gen) * dis * 8, 8.0f)];
    }
  }

  _cells.update(_verts);
}

void QuadTest::render(sf::RenderWindow& window)
{
  window.draw(_cells);
}

void QuadTest::onEnd()
{
  // clearing and recreating this onStart causes a crash. window likely holds a ref to VertexBuffer
  // _cells.create(0);
}