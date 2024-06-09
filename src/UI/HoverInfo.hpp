#pragma once

#include <SFML/Graphics.hpp>

#include "Utils/EventSystem/EventListener.hpp"

class HoverInfo : public sf::Drawable, public EventListener
{
public:
  HoverInfo(const sf::Font& font);
  virtual ~HoverInfo();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void setup();
  void show(const std::string& message, const sf::Vector2f& location);
  void hide();

private:
  sf::RectangleShape _background;
  sf::Text _message;
  bool _shouldShow;
};
