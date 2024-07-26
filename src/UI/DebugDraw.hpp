#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Utils/EventSystem/EventSystem.hpp"

class DebugDraw : public EventListener, public sf::Drawable
{
public:
  DebugDraw();
  ~DebugDraw();
  struct Line
  {
    sf::Vector2f _a;
    sf::Vector2f _b;
    sf::Color _color;
    float _sec;
  };
  void update(float sec);
  void line(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& color = sf::Color::White, const float& sec = -1.0f);
  void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
private:
  std::vector<Line> _lines;
};
