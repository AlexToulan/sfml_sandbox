#pragma once

#include "GameMode/GameMode.hpp"
#include "BoidManager.hpp"

class Boids : public GameMode
{
public:
  Boids();
  virtual ~Boids();
  virtual void onStart();
  virtual sf::Vector2f onResize(int screenX, int screenY);
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  BoidManager _boids;

  bool _bLeftMouse;
};
