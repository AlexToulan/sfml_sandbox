#include "BoidManager.hpp"
#include "Utils/Logging.hpp"
#include "Utils/MathUtils.hpp"
#include "Utils/VectorUtils.hpp"

#include <cmath>
#include <numeric>

BoidManager::BoidManager()
{
  _nearRange = 100.0f;
  _farRange = _nearRange + 100.0f;
  _outerRingDepth = _farRange - _nearRange;
  _attractForce = 1.0f;
  _repelForce = 10.0f;
  _targetSpeed = 100.0f;
}

BoidManager::~BoidManager()
{

}

bool BoidManager::loadAssets(const std::string& resources)
{
  std::string boidTexturePath = resources + "boid.png";
  if (!_boid.loadFromFile(boidTexturePath))
  {
    return false;
    Log().error("failed to load \"{}\"", boidTexturePath);
  }
  _sprite.setTexture(_boid);
  _sprite.setScale(0.25f, 0.25f);
  _sprite.setOrigin(32.0f, 32.0f);
  return true;
}

void BoidManager::update(float ds)
{
  for (auto a = 0ul; a < _physics.size(); a++)
  {
    std::vector<sf::Vector2f> neighborsAttract;
    std::vector<sf::Vector2f> neighborsRepel;
    sf::Vector2f neighborDir;
    int numNeighbors = 0;
    for (auto b = 0ul; b < _physics.size(); b++)
    {
      if (a == b)
        continue;

      auto [dis, normal] = vu::decompose(_positions[a] - _positions[b]);

      if (dis < _farRange)
      {
        if (dis < _nearRange)
        {
          // directly add force for repulsion
          _physics[a].addForce(normal * (_nearRange - dis) * 0.05f);
        }
        else
        {
          // store position to get group location later
          neighborsAttract.push_back(_positions[b]);
        }
        // alignment
        neighborDir += _physics[b].dir;
        numNeighbors++;
      }
    }

    // attract
    if (neighborsAttract.size() > 0)
    {
      auto averagePos = std::accumulate(
          neighborsAttract.begin(), neighborsAttract.end(), sf::Vector2f()) / float(neighborsAttract.size());
      auto groupCenterDir = vu::normalized(averagePos - _positions[a]);
      _physics[a].steerTowards(groupCenterDir, ds);
    }
    // alignment
    if (numNeighbors > 0)
    {
      _physics[a].steerTowards(vu::normalized(neighborDir), ds);
    }
  }

  for (auto i = 0ul; i < _physics.size(); i++)
  {
    _physics[i].applyForce();
    // TODO: lerp towards neighbor velocity
    _physics[i].vel = mu::lerp(_physics[i].vel, _targetSpeed, ds);
    _rotations[i] = atan2(_physics[i].dir.y, _physics[i].dir.x) * vu::toDegrees();
    // TODO: add drag based on turning angle
    // _physics[i].drag(ds, 0.4f);
    _physics[i].move(_positions[i], ds);
    _positions[i].x = mu::wrap(_positions[i].x, 0.0f, 1000.0f);
    _positions[i].y = mu::wrap(_positions[i].y, 0.0f, 1000.0f);
  }
}

void BoidManager::render(sf::RenderWindow& window)
{
  for (auto i = 0ul; i < _positions.size(); i++)
  {
    _sprite.setPosition(_positions[i]);
    _sprite.setRotation(_rotations[i]);
    window.draw(_sprite);
  }
}

void BoidManager::addBoid(sf::Vector2f pos, float rot)
{
  _positions.push_back(pos);
  _rotations.push_back(rot);
  _physics.push_back(BoidPhysics(vu::randomDir(), 1.75f));
}

void BoidManager::clear()
{
  _positions.clear();
  _rotations.clear();
  _physics.clear();
}
