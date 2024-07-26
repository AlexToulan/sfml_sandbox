#include "Agent.hpp"

Agent::Agent()
  : RectangleShape(sf::Vector2f(16.0f, 16.0f))
{
  setOrigin(getSize() * 0.5f);
  setFillColor(sf::Color(200, 200, 200));
}

Agent::~Agent()
{

}
