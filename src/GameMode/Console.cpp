#include "Console.hpp"
#include "Utils/Logging.hpp"

#include "EventTypes.hpp"

Console::Console(const sf::Font& font, int screenWidth, int screenHeight, int borderThickness)
  : _background(sf::Vector2f(screenWidth, screenHeight / 2))
{
  _backgroundColor = sf::Color(127, 127, 127, 127);
  _foregroundColor = sf::Color(192, 192, 192, 255);

  _background.setFillColor(_backgroundColor);
  _maxBufferLength = 128 * 1024;

  _bufferText.setFont(font);
  _bufferText.setFillColor(sf::Color::White);

  _bufferHintText.setFont(font);
  _bufferHintText.setFillColor(_foregroundColor);

  _outputText.setFont(font);
  _outputText.setFillColor(_foregroundColor);

  setSize(screenWidth, screenHeight, borderThickness);
  _prompt = " > ";
  _cursor = "_";
  _bIsOpen = false;
  _bCursorOn = true;
  _cursorSeconds = 0.5f;
  _cursorSecondsElapsed = _cursorSeconds;
  _historyIndex = 0;
}

Console::~Console()
{

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
  float margin = borderThickness + 4.0f;

  _anchor = sf::Vector2u(margin, margin + screenHeight / 2);
  _commandHistorySize = 128;
  _textBufferSize = (screenHeight - _anchor.y) / _charSize - 1;
  _bufferText.setCharacterSize(_charSize);
  _bufferText.setPosition(_anchor.x, screenHeight - _charSize - margin);
  _bufferHintText.setCharacterSize(_charSize);
  _bufferHintText.setPosition(_bufferText.getPosition());
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
    updateBufferText();
    updateOutputText();
  }
}

void Console::toggle()
{
  setOpen(!_bIsOpen);
}

void Console::updateBufferText()
{
  _bufferText.setString(_prompt + _commandBuffer + _cursor);

  // handle overflow
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

  updateHintText();
}

void Console::updateOutputText()
{
  //update output buffer
  std::string output;
  for (const auto& s : _textBuffer)
  {
      output += s + "\n";
  }
  _outputText.setString(output);
}

void Console::updateHintText()
{
  _hintCommand = "";
  if (_commandBuffer.size() != 0)
  {
    for(const std::string& command : _commands)
    {
      if (command.rfind(_commandBuffer, 0) == 0)
      {
        _hintCommand = command;
        break;
      }
    }
  }
  _bufferHintText.setString(_prompt + _hintCommand);
}

void Console::sendCommand()
{
  if (_commandBuffer.empty())
  {
    return;
  }

  // history
  _commandHistory.push_back(_commandBuffer);
  if (_commandHistory.size() > _commandHistorySize)
  {
    _commandHistory.erase(_commandHistory.begin());
  }
  _historyIndex = _commandHistory.size();

  // execute
  ConsoleCommand command(_commandBuffer);
  if (command._name == "help")
  {
    print("commands:");
    print(_commands, "  - ");
  }
  else if (std::find(_commands.begin(), _commands.end(), command._name) != _commands.end())
  {
    print("> " + _commandBuffer);
    EventSystem::publish(EventType::CONSOLE_COMMAND, Event(command));
  }
  else
  {
    print("invalid command: \"" + _commandBuffer + "\"");
    print("\tuse \"help\" for command list");
  }

  // finalize
  _commandBuffer.clear();
  _hintCommand.clear();
  _tempCommandBuffer.clear();
  updateBufferText();
  updateOutputText();
}

void Console::print(const std::string line)
{
  _textBuffer.push_back(line);
  if (_textBuffer.size() > _textBufferSize)
  {
    _textBuffer.erase(_textBuffer.begin());
  }
}

void Console::print(const std::vector<std::string> lines, const std::string newlinePrefix)
{
  std::for_each(lines.begin(), lines.end(),
    [&](const std::string& line)
    {
      _textBuffer.push_back(newlinePrefix + line);
      if (_textBuffer.size() > _textBufferSize)
      {
        _textBuffer.erase(_textBuffer.begin());
      }
    }
  );
}

void Console::processEvents(sf::Event& event)
{
  if (!_bIsOpen)
  {
    return;
  }

  if (event.type == sf::Event::KeyPressed)
  {
    // temp activate cursor
    _cursorSecondsElapsed = 0.0f;
    _cursor = "_";
    _bCursorOn = true;
    // process keys
    switch ((int)event.key.code)
    {
    case sf::Keyboard::BackSpace:
      if (!_commandBuffer.empty())
      {
        _commandBuffer.pop_back();
      }
      updateBufferText();
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
          updateBufferText();
        }
      }
      return;
    case sf::Keyboard::Tab:
      if (_hintCommand.size() > 0)
      {
        _commandBuffer = _hintCommand;
        updateBufferText();
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
          updateBufferText();
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
      updateBufferText();
    }
  }
}

void Console::update(float deltaSeconds)
{
  _cursorSecondsElapsed += deltaSeconds;
  if (_cursorSecondsElapsed > _cursorSeconds)
  {
    _cursorSecondsElapsed -= _cursorSeconds;
    _bCursorOn = !_bCursorOn;
    if (_bCursorOn)
    {
      _cursor = "_";
      _bufferText.setString(_prompt + _commandBuffer + _cursor);
    }
    else
    {
      _cursor = "";
      _bufferText.setString(_prompt + _commandBuffer + _cursor);
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
  rt.draw(_bufferHintText, states);
  rt.draw(_bufferText, states);
}

void Console::addCommand(const EventBase& event)
{
  addCommand(unpack<std::string>(event));
}

void Console::addCommand(const std::string command)
{
  if (std::find(_commands.begin(), _commands.end(), command) == _commands.end())
  {
    auto it = std::lower_bound(_commands.begin(), _commands.end(),
      command, std::less<std::string>());
    _commands.insert(it, command);
  }
  Log::info(command + std::to_string(_commands.size()));
}