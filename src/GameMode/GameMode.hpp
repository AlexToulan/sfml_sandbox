#pragma once

#include <SFML/Graphics.hpp>
#include <source_location>

class GameMode
{
public:
  GameMode(const std::string& filePath = std::source_location::current().file_name());
  virtual ~GameMode();
  // called when the game-mode starts
  virtual void onStart() = 0;
  // returns the view adjustment for window resizing
  virtual sf::Vector2f onResize(int screenX, int screenY) = 0;
  // processes input events from sfml
  virtual void processEvents(sf::Event& event) = 0;
  // called before render, passes the accumulated seconds elapsed since the previous update call
  virtual void update(float deltaSeconds) = 0;
  // renders elements of the game-mode
  virtual void render(sf::RenderWindow& window) = 0;
  // called when the game-mode ends
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