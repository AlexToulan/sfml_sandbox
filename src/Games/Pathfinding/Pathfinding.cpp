#include "Pathfinding.hpp"

Pathfinding::Pathfinding(sf::Vector2u originalScreenSize)
 : GameMode(originalScreenSize)
{
}

Pathfinding::~Pathfinding()
{

}

void Pathfinding::onStart()
{

}

void Pathfinding::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      // event.mouseButton.x, event.mouseButton.y
    }
  }
}

void Pathfinding::update(float ds)
{
}

void Pathfinding::render(sf::RenderWindow& window)
{
}

void Pathfinding::onEnd()
{
}
