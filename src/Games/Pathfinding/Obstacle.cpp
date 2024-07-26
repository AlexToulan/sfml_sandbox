#include <limits.h>

#include "Obstacle.hpp"
#include "Utils/Logging.hpp"

Obstacle::Obstacle(const std::vector<sf::Vector2f>& points)
{
  _points = points;
  _center = sf::Vector2f();
  for (auto point : _points)
  {
    _center += point;
  }
  _center /= (float)_points.size();
}

Obstacle::~Obstacle()
{

}

std::pair<bool, sf::Vector2f> Obstacle::getCollision(const sf::Vector2f& a, const sf::Vector2f& b) const
{
  ray::Hit hit;
  for(size_t i = 1; i < _points.size(); i++)
  {
    ray::cast(hit, a, b, _points[i - 1], _points[i]);
    Log().debug("hit: {}\t[{}, {}]", hit.collision ? "hit" : "no hit", hit.point.x, hit.point.y);
  }

  // return no-hit
  return std::make_pair(hit.collision, hit.point);
}
