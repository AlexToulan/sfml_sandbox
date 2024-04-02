#pragma once

#include "BoidPhysics.hpp"

#include <vector>

class Boids
{
public:
  Boids();
  ~Boids();
  void update(float ds);
  void render(sf::RenderWindow& window);
  void addBoid(sf::Vector2f pos, float rotation = 0.0f);

private:
  std::vector<sf::Vector2f> _positions;
  std::vector<float> _rotations;
  std::vector<sf::Texture> _textures;
  std::vector<BoidPhysics> _physics;
};
