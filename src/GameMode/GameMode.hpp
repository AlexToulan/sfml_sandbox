#pragma once

#include <SFML/Graphics.hpp>

class GameMode
{
public:
  GameMode();
  GameMode(const std::string& filePath);
  virtual ~GameMode();
  virtual void onStart() = 0;
  virtual void onResize(int screenX, int screenY) = 0;
  virtual void processEvents(sf::Event& event) = 0;
  // called before render passing the seconds elapsed during the previous loop
  virtual void update(float deltaSeconds) = 0;
  virtual void render(sf::RenderWindow& window) = 0;
  virtual void onEnd() = 0;

  std::string getName() const;
  sf::Color getClearColor() const;

protected:
  std::string _resourcesPath;
  sf::Vector2i _screenSize;
  sf::Color _clearColor;

private:
  std::string _name;
};