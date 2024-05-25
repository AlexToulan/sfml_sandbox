#pragma once

#include "BoidPhysics.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class BoidManager
{
public:
  BoidManager();
  ~BoidManager();
  bool loadAssets(const std::string& resources);
  void update(float ds);
  void render(sf::RenderWindow& window);
  void addBoid(sf::Vector2f pos, float rot = 0.0f);
  void clear();
  void setAreaSize(sf::Vector2u size);

private:
  sf::Texture _boid;
  sf::Sprite _sprite;
  std::vector<sf::Vector2f> _positions;
  std::vector<float> _rotations;
  std::vector<BoidPhysics> _physics;
  sf::Vector2f _areaSize;

  float _nearRange;
  float _farRange;
  float _outerRingDepth;
  float _attractForce;
  float _repelForce;
  float _targetSpeed;
};
