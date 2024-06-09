#include "Pathfinding.hpp"
#include "Utils/Logging.hpp"

Pathfinding::Pathfinding(sf::Vector2u originalScreenSize)
 : GameMode(originalScreenSize)
{
}

Pathfinding::~Pathfinding()
{

}

void Pathfinding::onStart()
{
  _grid.setup(_originalScreenSize, 25, 1);
}

void Pathfinding::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      auto mousePos = getMousePos(event.mouseButton.x, event.mouseButton.y);
    }
  }
  if (event.type == sf::Event::MouseMoved)
  {
    _grid.hover(getMousePos(event.mouseMove.x, event.mouseMove.y));
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
  {
    _grid.save("grid.png");
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
  {
    _grid.load("grid.png");
  }
}

void Pathfinding::update(float ds)
{
}

void Pathfinding::render(sf::RenderWindow& window)
{
  window.draw(_grid);
}

void Pathfinding::onEnd()
{
}
