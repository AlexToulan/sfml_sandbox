#include "Pathfinding.hpp"

Pathfinding::Pathfinding()
 : GameMode() // (std::source_location::current().file_name())
{

}

Pathfinding::~Pathfinding()
{

}

void Pathfinding::onStart()
{
}

sf::Vector2f Pathfinding::onResize(int screenX, int screenY)
{
  float minScreen = std::min(screenX, screenY);
  return sf::Vector2f();
}

void Pathfinding::processEvents(sf::Event& event)
{
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
