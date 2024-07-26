#pragma once

#include <vector>

#include "Utils/VectorUtils.hpp"

class Obstacle
{
public:
  Obstacle(const std::vector<sf::Vector2f>& points);
  ~Obstacle();
  std::pair<bool, sf::Vector2f> getCollision(const sf::Vector2f& a, const sf::Vector2f& b) const;
private:
  sf::Vector2f _center;
  std::vector<sf::Vector2f> _points;
};
