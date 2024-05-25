#include "Boids.hpp"

Boids::Boids(sf::Vector2u originalScreenSize)
 : GameMode(originalScreenSize)
{
}

Boids::~Boids()
{

}

void Boids::onStart()
{
  _clearColor = sf::Color(32, 32, 32);
  _bLeftMouse = false;
  _boids.loadAssets(_resourcesPath);
  srand(0);
  for (int i = 0; i < 5; i++)
    _boids.addBoid(sf::Vector2f(500, 500) + vu::randomDir() * 5.0f, -90.0f);
}

void Boids::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      _boids.addBoid(sf::Vector2f(event.mouseButton.x - _viewOffset.x, event.mouseButton.y - _viewOffset.y));
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
