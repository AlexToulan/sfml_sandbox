#pragma once

#include <vector>

#include "Obstacle.hpp"
#include "PathGraph.hpp"
#include "Utils/VectorUtils.hpp"

class RayPath
{
public:
  RayPath();
  ~RayPath();
  bool loadMap(std::string yamlFileName);
  void addObstacle(const std::vector<sf::Vector2f>& points);
  std::pair<bool, sf::Vector2f> avoidancePath(sf::Vector2f a, sf::Vector2f b);
  std::vector<sf::Vector2f> calculateCompletePath(sf::Vector2f a, sf::Vector2f b);
private:
  std::vector<Obstacle> _obstacles;
  void _calculateCompletePath(sf::Vector2f a, sf::Vector2f b, std::vector<sf::Vector2f>& outPath);
};
