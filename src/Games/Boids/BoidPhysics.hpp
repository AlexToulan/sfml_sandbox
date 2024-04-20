#pragma once

#include <SFML/System/Vector2.hpp>
#include "Utils/MathUtils.hpp"
#include "Utils/VectorUtils.hpp"
#include "Utils/Logging.hpp"

struct BoidPhysics
{
  BoidPhysics(sf::Vector2f v = sf::Vector2f(), float m = 1.0f)
  {
    std::tie(vel, dir) = vu::decompose(v);
    forceAccumulator = sf::Vector2f();
    setMass(m);
  }

  void setMass(float m)
  {
    mass = std::abs(m);
    if (mass == 0.0f)
      invMass = 0.0f;
    else
      invMass = 1.0f / mass;
  }

  void addForce(const sf::Vector2f& force)
  {
    forceAccumulator += force;
  }

  void steerTowards(const sf::Vector2f& targetDir, float ds)
  {
    float scale = std::min(0.4f * ds, 0.5f);

    if (vu::dot(dir, targetDir) > 0.0f)
    {
      dir = vu::normalized(mu::lerp(dir, targetDir, scale));
    }
    else
    {
      // Log().debug("[{} {}] [{} {}]", dir.x, dir.y, targetDir.x, targetDir.y);
      dir = vu::normalized(mu::lerp(dir, targetDir + dir, scale * 2.0f));
    }
  }

  void applyForce()
  {
    auto velocity = dir * vel;
    velocity += forceAccumulator * invMass;
    std::tie(vel, dir) = vu::decompose(velocity);
    forceAccumulator = sf::Vector2f();
  }

  void move(sf::Vector2f& outPos, float ds)
  {
    outPos += dir * vel * ds;
  }

  void drag(float ds, float dragCoefficient = 0.4f)
  {
    // Fd: Drag Force
    // p:  fluid density
    // v:  Velocity
    // Cd: Drag Coefficient
    // A:  Cross-sectional Area
    // Vd: Delta Velocity
    // Td: Delta Time
    // Fd = 1/2 p v^2 Cd A

    // Formulation:
    //   Refactor:       Fd = 1/2 v v Cd A p
    //   Expand Force:   m Dv / Td = 1/2 v v Cd A p
    //   Refactor:       Vd = (Td v v Cd A p) / 2m
  
    // p 1.2 kg/meter^3 (air)
    // A 0.875 meter^3  (gull)
    static float Ap = 1.2f * 0.875f; // 1.05f
    //     Td * v *   v *   Cd *            A * p  /      2m
    vel -= ds * vel * vel * dragCoefficient * Ap * invMass * 0.5f;
  }

  sf::Vector2f dir;
  float vel;
  sf::Vector2f forceAccumulator;
  float mass;
  float invMass;
};
