#include "Boids.hpp"

Boids::Boids()
 : GameMode()
{
}

Boids::~Boids()
{

}

void Boids::onStart()
{
  _bLeftMouse = false;
  _boids.loadAssets(_resourcesPath);
  srand(0);
  for (int i = 0; i < 5; i++)
    _boids.addBoid(sf::Vector2f(500, 500) + vu::randomDir() * 5.0f, -90.0f);
}

sf::Vector2f Boids::onResize(int screenX, int screenY)
{
  _screenSize = sf::Vector2i(screenX, screenY);
  Log().debug("{} {}", _screenSize.x, _screenSize.y);
  return sf::Vector2f(std::max(screenX - screenY, 0), std::max(screenY - screenX, 0)) * 0.5f;
}

void Boids::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      _boids.addBoid(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
    }
  }
}

void Boids::update(float ds)
{
  _boids.update(ds);
}

void Boids::render(sf::RenderWindow& window)
{
  _boids.render(window);
}

void Boids::onEnd()
{
  _boids.clear();
}
