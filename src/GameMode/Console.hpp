#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include "Utils/CliConfig.hpp"

class Console final : public sf::Drawable, private sf::NonCopyable
{
public:
  Console(const sf::Font& font, int screenWidth = 1000, int screenHeight = 1000, int borderThickness = 0);
  ~Console() = default;
  void setSize(int screenWidth, int screenHeight, int borderThickness);
  bool isOpen();
  void setOpen(bool isOpen);
  void toggle();
  void processEvents(sf::Event& event);
  void render(sf::RenderWindow& window);

private:
  void updateText();
  void sendCommand();
  void print(const std::string line);
  void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

  bool _bIsOpen;
  // UI
  sf::Vector2u _anchor;
  sf::RectangleShape _background;
  sf::Text _bufferText;
  sf::Text _outputText;
  sf::Color _backgroundColor;
  sf::Color _foregroundColor;

  // 
  int _charSize;
  size_t _commandHistorySize;
  size_t _textBufferSize;
  size_t _maxBufferLength;
  std::string _prompt;
  std::string _commandBuffer;
  std::string _tempCommandBuffer;
  int _historyIndex;
  std::vector<std::string> _commandHistory;
  std::vector<std::string> _textBuffer;
};
