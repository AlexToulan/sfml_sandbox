#pragma once

#include "GameMode/GameMode.hpp"
#include "Boids.hpp"

class Pathfinding : public GameMode
{
public:
  Pathfinding();
  virtual ~Pathfinding();
  virtual void onStart();
  virtual sf::Vector2f onResize(int screenX, int screenY);
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  Boids _boids;
};
