#include "GameMode.hpp"

GameMode::GameMode(sf::Vector2u originalScreenSize, const std::string& filePath)
{
  _clearColor = sf::Color::Black;
  size_t start = filePath.find_last_of("/") + 1;
  size_t end = filePath.find_last_of(".");
  _name = filePath.substr(start, end - start);

  _resourcesPath = "resources/" + _name + "/";
  _originalScreenSize = originalScreenSize;
  _viewOffset = sf::Vector2f();
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

sf::Vector2u GameMode::getOriginalScreenSize() const
{
  return _originalScreenSize;
}

sf::Vector2f GameMode::getMousePos(int x, int y)
{
  return sf::Vector2f(x - _viewOffset.x, y - _viewOffset.y);
}

sf::Vector2f GameMode::resizeStretch(int screenX, int screenY)
{
  _screenSize = sf::Vector2u(screenX, screenY);
  _viewOffset = sf::Vector2f();
  return _viewOffset;
}

sf::Vector2f GameMode::resizeCenter(int screenX, int screenY)
{
  _screenSize = sf::Vector2u(screenX, screenY);
  _viewOffset = sf::Vector2f(screenX - _originalScreenSize.x, screenY - _originalScreenSize.y) * 0.5f;
  return _viewOffset;
}
