#include "GameMode/Console.hpp"

#include "EventTypes.hpp"
#include "Utils/EventSystem/EventComponent.hpp"

Console::Console(const sf::Font& font, int screenWidth, int screenHeight, int borderThickness)
  : _background(sf::Vector2f(screenWidth, screenHeight / 2))
{
  _backgroundColor = sf::Color(127, 127, 127, 127);
  _foregroundColor = sf::Color(192, 192, 192, 127);
  _background.setFillColor(_backgroundColor);
  _maxBufferLength = 128 * 1024;
  _bufferText.setFont(font);
  _outputText.setFont(font);
  setSize(screenWidth, screenHeight, borderThickness);
  _prompt = " > ";
  _bIsOpen = false;
  _historyIndex = 0;
  // add commands
  _commands.push_back("exit");
  _commands.push_back("quit");
}

void Console::setSize(int screenWidth, int screenHeight, int borderThickness)
{
  screenWidth -= borderThickness * 2;
  screenHeight -= borderThickness * 2;
  _background.setSize(sf::Vector2f(screenWidth, screenHeight / 2));
  if (borderThickness > 0)
  {
    _background.setOutlineThickness(borderThickness);
    _background.setOutlineColor(_foregroundColor);
  }
  _background.setPosition(borderThickness, borderThickness + screenHeight - screenHeight / 2);

  _charSize = 16u;
  float margin = 0.0f + borderThickness / 2;

  _anchor = sf::Vector2u(margin, margin + screenHeight / 2);
  _commandHistorySize = 128;
  _textBufferSize = (screenHeight - _anchor.y) / _charSize - 1;
  _bufferText.setCharacterSize(_charSize);
  _bufferText.setPosition(_anchor.x, screenHeight - _charSize - margin - 2);
  _outputText.setCharacterSize(_charSize);
  _outputText.setPosition(_anchor.x, _anchor.y);
}

bool Console::isOpen()
{
  return _bIsOpen;
}

void Console::setOpen(bool isOpen)
{
  _bIsOpen = isOpen;
  if (_bIsOpen)
  {
    updateText();
  }
}

void Console::toggle()
{
  setOpen(!_bIsOpen);
}

void Console::updateText()
{
  _bufferText.setString(_prompt + _commandBuffer + "_");
  sf::Vector2f bufferPos = _bufferText.getPosition();
  float screenSpaceOverflow = _bufferText.getLocalBounds().width - _background.getSize().x;
  if (screenSpaceOverflow > 0.0f)
  {
    _bufferText.setPosition(-screenSpaceOverflow, bufferPos.y);
  }
  else
  {
    _bufferText.setPosition(_anchor.x, bufferPos.y);
  }
  //update output buffer
  std::string output;
  for (const auto& s : _textBuffer)
  {
      output += s + "\n";
  }
  _outputText.setString(output);
}

void Console::sendCommand()
{
  if (_commandBuffer.empty())
  {
    return;
  }
  _commandHistory.push_back(_commandBuffer);
  if (_commandHistory.size() > _commandHistorySize)
  {
    _commandHistory.erase(_commandHistory.begin());
  }
  _historyIndex = _commandHistory.size();
  print("> " + _commandBuffer);
  ConsoleCommand command(_commandBuffer);
  if (std::find(_commands.begin(), _commands.end(), command._name) != _commands.end())
  {
    EventComponent::publish(EventType::CONSOLE_COMMAND, Event(command));
  }
  else
  {
    print("invalid command: \"" + _commandBuffer + "\"");
  }
  _commandBuffer.clear();
  _tempCommandBuffer.clear();
  updateText();
}

void Console::print(const std::string line)
{
  _textBuffer.push_back(line);
  if (_textBuffer.size() > _textBufferSize)
  {
    _textBuffer.erase(_textBuffer.begin());
  }
}

void Console::processEvents(sf::Event& event)
{
  if (!_bIsOpen)
  {
    return;
  }

  if (event.type == sf::Event::KeyPressed)
  {
    switch ((int)event.key.code)
    {
    case sf::Keyboard::BackSpace:
      if (!_commandBuffer.empty())
      {
        _commandBuffer.pop_back();
      }
      updateText();
      return;
    case sf::Keyboard::Return:
      sendCommand();
      return;
    case sf::Keyboard::Up:
      if (!_commandHistory.empty())
      {
        if (_historyIndex > 0)
        {
          if (_historyIndex == _commandHistory.size())
          {
            _tempCommandBuffer = _commandBuffer;
          }
          _commandBuffer = _commandHistory[--_historyIndex];
          updateText();
        }
      }
      return;
    case sf::Keyboard::Down:
      if (!_commandHistory.empty())
      {
        if (_historyIndex < _commandHistory.size())
        {
          _historyIndex++;
          if (_historyIndex == _commandHistory.size())
          {
            _commandBuffer = _tempCommandBuffer;
          }
          else
          {
            _commandBuffer = _commandHistory[_historyIndex];
          }
          updateText();
        }
      }
      return;
    }
  }
  else if (event.type == sf::Event::TextEntered)
  {
    // handle ASCII characters only. skipping grave, backspace and delete
    if (event.text.unicode != 96 && event.text.unicode > 31 && event.text.unicode < 127
      && _commandBuffer.size() < _maxBufferLength)
    {
      _commandBuffer += static_cast<char>(event.text.unicode);
      updateText();
    }
  }
}

void Console::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
  if (!_bIsOpen)
  {
    return;
  }
  rt.draw(_background, states);
  rt.draw(_outputText, states);
  rt.draw(_bufferText, states);
}
