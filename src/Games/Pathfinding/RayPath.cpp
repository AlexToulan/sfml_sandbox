#include <yaml-cpp/yaml.h>

#include "GameMode/DebugEvents.hpp"
#include "RayPath.hpp"
#include "Utils/Logging.hpp"

RayPath::RayPath()
{

}

RayPath::~RayPath()
{

}

bool RayPath::loadMap(std::string yamlFileName)
{
  YAML::Node root = YAML::LoadFile(yamlFileName);
  if (!root["obstacles"] || !root["obstacles"].IsSequence())
  {
    Log().error("Could not load " + yamlFileName);
    return false;
  }

  for (auto obstacle = root["obstacles"].begin(); obstacle != root["obstacles"].end(); ++obstacle)
  {
    if (!obstacle->IsSequence())
    {
      Log().warn("ill-formed obstacle sequence");
      continue;
    }
    std::vector<sf::Vector2f> points;
    for (auto point = obstacle->begin(); point != obstacle->end(); ++point)
    {
      points.push_back(sf::Vector2f((*point)[0].as<float>(0.0f), (*point)[1].as<float>(0.0f)));
    }
    for (size_t i = 1; i < points.size(); i++)
    {
      Events::Debug->publish(EDebugEvents::LINE, points[i - 1], points[i], sf::Color::White, 6000.0f);
    }
    Events::Debug->publish(EDebugEvents::LINE, points[0], points[points.size() - 1], sf::Color::White, 6000.0f);
    addObstacle(points);
  }
  return true;
}

void RayPath::addObstacle(const std::vector<sf::Vector2f>& points)
{
  _obstacles.push_back(Obstacle(points));
}

std::pair<bool, sf::Vector2f> RayPath::avoidancePath(sf::Vector2f a, sf::Vector2f b)
{
  // collision with walls from start to end
  std::vector<std::pair<bool, sf::Vector2f>> collisions;
  for(const auto& obstacle : _obstacles)
  {
    auto collision = obstacle.getCollision(a, b);
    if (collision.first)
      collisions.push_back(collision);
  }

  if (collisions.size() == 0)
    return std::make_pair(false, sf::Vector2f());

  // get closest collision
  float minDisSqr = vu::float_max;
  size_t minIt = 0;
  for(size_t i = 0; i < collisions.size(); i++)
  {
    float disSqr = vu::lengthSquared(collisions[i].second - a);
    if (disSqr < minDisSqr)
    {
      minDisSqr = disSqr;
      minIt = i;
    }
  }
  return collisions[minIt];
}

std::vector<sf::Vector2f> RayPath::calculateCompletePath(sf::Vector2f a, sf::Vector2f b)
{
  std::vector<sf::Vector2f> path;
  _calculateCompletePath(a, b, path);
  path.push_back(b);
  return path;
}

void RayPath::_calculateCompletePath(sf::Vector2f a, sf::Vector2f b, std::vector<sf::Vector2f>& outPath)
{
  auto pathSegment = avoidancePath(a, b);
  if (pathSegment.first)
  {
    // recurse down near paths
    _calculateCompletePath(a, pathSegment.second, outPath);
    // add the nearest collision avoidance path
    outPath.push_back(pathSegment.second);
    // recurse
    _calculateCompletePath(pathSegment.second, b, outPath);
  }
}
