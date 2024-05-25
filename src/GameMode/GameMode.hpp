#pragma once

#include <SFML/Graphics.hpp>
#include <source_location>

class GameMode
{
public:
  GameMode(sf::Vector2u originalScreenSize = sf::Vector2u(1000, 1000), const std::string& filePath = std::source_location::current().file_name());
  virtual ~GameMode();
  // called when the game-mode starts
  virtual void onStart() = 0;
  // returns the view adjustment for window resizing
  virtual sf::Vector2f onResize(int screenX, int screenY);
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
  sf::Vector2u getOriginalScreenSize() const;

protected:
  sf::Vector2f resizeStretch(int screenX, int screenY);
  sf::Vector2f resizeCenter(int screenX, int screenY);

  std::string _resourcesPath;
  sf::Vector2u _originalScreenSize;
  sf::Vector2u _screenSize;
  sf::Vector2f _viewOffset;
  sf::Color _clearColor;

private:
  std::string _name;
};
