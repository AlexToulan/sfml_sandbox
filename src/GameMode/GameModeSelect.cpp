#include "GameModeSelect.hpp"
#include "Utils/Logging.hpp"
#include "ConsoleEvents.hpp"

GameModeSelect::GameModeSelect(const sf::Font& font, sf::Vector2u originalScreenSize)
 : GameMode(originalScreenSize)
{
  _font = font;
}

GameModeSelect::~GameModeSelect()
{

}

void GameModeSelect::onStart()
{
  auto center = _originalScreenSize / 2u;
  float padding = 12.0f;
  float spacing = _characterSize + padding;
  float height = (float)_gameModes.size() * spacing;
  float y = center.y - height * 0.75f;
  for (auto& item : _gameModes)
  {
    item.setOutlineThickness(2.0f);
    // item.setOutlineColor(sf::Color::White);
    item.setOutlineColor(sf::Color(200, 200, 200, 127));
    item.setFillColor(sf::Color(200, 200, 200));
    item.setPosition(center.x / 2, y);
    y += spacing;
  }
}

void GameModeSelect::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      auto mousePos = getMousePos(event.mouseButton.x, event.mouseButton.y);
      for (auto& item : _gameModes)
      {
        if (item.getGlobalBounds().contains(mousePos))
        {
          Events::Console->publish("select_game_mode", item.getString().toAnsiString());
        }
      }
    }
  }
  if (event.type == sf::Event::MouseMoved)
  {
    auto mousePos = getMousePos(event.mouseMove.x, event.mouseMove.y);
    for (auto& item : _gameModes)
    {
      if (item.getGlobalBounds().contains(mousePos))
      {
        item.setOutlineThickness(1.0f);
      }
      else
      {
        item.setOutlineThickness(2.0f);
      }
    }
  }
}

void GameModeSelect::update(float ds)
{
}

void GameModeSelect::render(sf::RenderWindow& window)
{
  for (const auto& item : _gameModes)
  {
    window.draw(item);
  }
}

void GameModeSelect::onEnd()
{
}

void GameModeSelect::addGameMode(std::string gameModeName)
{
  _gameModes.push_back(sf::Text(gameModeName, _font, _characterSize));
}
