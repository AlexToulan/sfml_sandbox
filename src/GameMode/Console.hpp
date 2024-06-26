#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include "Utils/EventSystem/EventListener.hpp"

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

class Console final : public EventListener, public sf::Drawable
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
  void notify(const std::string& line);

  // events
  void addCommand(const std::string& command, const std::string& help);

private:
  void updateBufferText();
  void updateOutputText();
  void updateHintText();
  void sendCommand();
  void print(const std::string& line);
  void print(const std::vector<std::string>& lines, const std::string newlinePrefix = "");
  void print(const std::map<std::string, std::string>& dic, const std::string newlinePrefix = "", size_t secondColumnOffset = 0);
  void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

  // commands
  std::map<std::string, std::string> _commands;
  size_t _longestCommandName;

  // UI
  bool _bIsOpen;
  int _charSize;
  sf::Vector2i _anchor;
  sf::RectangleShape _background;
  sf::Color _backgroundColor;
  sf::Color _foregroundColor;
  std::string _prompt;
  float _cursorSeconds;
  float _cursorSecondsElapsed;
  bool _bCursorOn;
  std::string _cursor;
  sf::Vector2i _originalScreenSize;
  // notify
  sf::Text _notifyText;
  sf::Vector2f _notifyAnchor;
  float _notifySeconds;
  float _notifyFadeSeconds;
  std::vector<std::pair<std::string, float>> _notifyMessages;

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
