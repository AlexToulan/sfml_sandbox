#include "DebugDraw.hpp"
#include "GameMode/DebugEvents.hpp"

DebugDraw::DebugDraw()
  : EventListener()
{
}

DebugDraw::~DebugDraw()
{

}

void DebugDraw::update(float sec)
{
  _lines.erase(
   std::remove_if(
      _lines.begin(), _lines.end(),
      [&](Line& l)
      {
        if (l._sec == -1.0f)
          return false;
        l._sec -= sec;
        return l._sec <= 0.0f;
      }),
   _lines.end());
}

void DebugDraw::line(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& color, const float& sec)
{
  _lines.push_back(Line(a, b, color, sec));
}

void DebugDraw::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
  for (const auto& line : _lines)
  {
    sf::Vertex v[] = {{{line._a.x, line._a.y}, line._color}, {{line._b.x, line._b.y}, line._color}};
    rt.draw(v, 2, sf::Lines, states);
  }
}
