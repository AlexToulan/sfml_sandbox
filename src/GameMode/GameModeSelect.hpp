#pragma once

#include "GameMode/GameMode.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class GameModeSelect : public GameMode
{
public:
  GameModeSelect(const sf::Font& font, sf::Vector2u originalScreenSize = sf::Vector2u(1000, 1000));
  virtual ~GameModeSelect();
  virtual void onStart();
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

  void addGameMode(std::string gameModeName);

private:
  unsigned int _characterSize = 42;
  sf::Font _font;
  std::vector<sf::Text> _gameModes;
};
