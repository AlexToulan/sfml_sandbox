#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

struct ConsoleCommand
{
  ConsoleCommand(std::string commandBuffer)
  {
    size_t space = commandBuffer.find_first_of(" \t");
    if (space == std::string::npos)
    {
      _name = commandBuffer;
      _arg = "";
    }
    else
    {
      _name = commandBuffer.substr(0, space);
      _arg = commandBuffer.substr(space + 1, commandBuffer.size());
    }
  }
  std::string _name;
  std::string _arg;
};

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

  // commands
  std::vector<std::string> _commands;

  // UI
  bool _bIsOpen;
  int _charSize;
  sf::Vector2u _anchor;
  sf::RectangleShape _background;
  sf::Color _backgroundColor;
  sf::Color _foregroundColor;

  // command buffer
  sf::Text _bufferText;
  std::string _prompt;
  std::string _commandBuffer;
  std::string _tempCommandBuffer;
  size_t _maxBufferLength;

  // output text
  sf::Text _outputText;
  std::vector<std::string> _textBuffer;
  size_t _textBufferSize;

  // history
  int _historyIndex;
  std::vector<std::string> _commandHistory;
  size_t _commandHistorySize;
};
