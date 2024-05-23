#include "GameMode.hpp"

GameMode::GameMode(sf::Vector2i originalScreenSize, const std::string& filePath)
{
  _clearColor = sf::Color::Black;
  size_t start = filePath.find_last_of("/") + 1;
  size_t end = filePath.find_last_of(".");
  _name = filePath.substr(start, end - start);

  _resourcesPath = "resources/" + _name + "/";
  _originalScreenSize = originalScreenSize;
}

GameMode::~GameMode()
{

}

sf::Vector2f GameMode::onResize(int screenX, int screenY)
{
  return resizeCenter(screenX, screenY);
}

std::string GameMode::getName() const
{
  return _name;
}

sf::Color GameMode::getClearColor() const
{
  return _clearColor;
}

sf::Vector2f GameMode::resizeCenter(int screenX, int screenY)
{
  _screenSize = sf::Vector2i(screenX, screenY);
  _viewOffset = sf::Vector2f(screenX - _originalScreenSize.x, screenY - _originalScreenSize.y) * 0.5f;
  return _viewOffset;
}
