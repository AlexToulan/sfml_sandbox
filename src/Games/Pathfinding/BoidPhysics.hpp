#pragma once

#include <SFML/System/Vector2.hpp>

struct BoidPhysics
{
  BoidPhysics()
  {
    vel = sf::Vector2f();
    acc = sf::Vector2f();
    invMass = 1.0f;
  }

  void setMass(float mass)
  {
    invMass = 1.0f / mass;
  }

  void addForce(const sf::Vector2f& force)
  {
    acc += force * invMass;
  }

  void applyForce()
  {
    vel += acc;
    acc = sf::Vector2f();
  }

  void move(sf::Vector2f& outPos, float ds)
  {
    outPos += vel * ds;
  }

  sf::Vector2f vel;
  sf::Vector2f acc;
  float invMass;
};