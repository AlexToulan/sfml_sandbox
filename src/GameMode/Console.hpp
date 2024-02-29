#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include "Utils/EventSystem/EventComponent.hpp"

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

class Console final : public sf::Drawable, public EventComponent
{
public:
  Console(const sf::Font& font, int screenWidth = 1000, int screenHeight = 1000, int borderThickness = 0);
  virtual ~Console();
  void setSize(int screenWidth, int screenHeight, int borderThickness);
  bool isOpen();
  void setOpen(bool isOpen);
  void toggle();
  void processEvents(sf::Event& event);
  void update(float deltaSeconds);

  // events
  void addCommand(const EventBase& event);
  void addCommand(const std::string command);

private:
  void updateBufferText();
  void updateOutputText();
  void updateHintText();
  void sendCommand();
  void print(const std::string line);
  void print(const std::vector<std::string> lines, const std::string newlinePrefix = "");
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
  std::string _prompt;
  float _cursorSeconds;
  float _cursorSecondsElapsed;
  bool _bCursorOn;
  std::string _cursor;

  // command buffer
  sf::Text _bufferText;
  std::string _commandBuffer;
  std::string _tempCommandBuffer;
  size_t _maxBufferLength;

  // hint text
  sf::Text _bufferHintText;
  std::string _hintCommand;

  // output text
  sf::Text _outputText;
  std::vector<std::string> _textBuffer;
  size_t _textBufferSize;

  // history
  int _historyIndex;
  std::vector<std::string> _commandHistory;
  size_t _commandHistorySize;
};
