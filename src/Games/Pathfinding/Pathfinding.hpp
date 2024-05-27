#pragma once

#include "GameMode/GameMode.hpp"

class Pathfinding : public GameMode
{
public:
  Pathfinding(sf::Vector2u originalScreenSize = sf::Vector2u(1000, 1000));
  virtual ~Pathfinding();
  virtual void onStart();
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  bool _bLeftMouse;
};
