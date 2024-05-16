#include "GameMode.hpp"

GameMode::GameMode(const std::string& filePath)
{
  _clearColor = sf::Color::Black;
  size_t start = filePath.find_last_of("/") + 1;
  size_t end = filePath.find_last_of(".");
  _name = filePath.substr(start, end - start);

  _resourcesPath = "resources/" + _name + "/";
}

GameMode::~GameMode()
{

}

sf::Vector2f GameMode::onResize(int screenX, int screenY)
{
  _screenSize = sf::Vector2i(screenX, screenY);
  return sf::Vector2f(std::max(screenX - screenY, 0), std::max(screenY - screenX, 0)) * 0.5f;
}

std::string GameMode::getName() const
{
  return _name;
}

sf::Color GameMode::getClearColor() const
{
  return _clearColor;
}
