#include "Console.hpp"
#include "Utils/Logging.hpp"
#include "ConsoleEvents.hpp"

Console::Console(const sf::Font& font, int screenWidth, int screenHeight, int borderThickness)
  : _background(sf::Vector2f(screenWidth, screenHeight / 2))
{
  _backgroundColor = sf::Color(127, 127, 127, 127);
  _foregroundColor = sf::Color(192, 192, 192, 255);

  _background.setFillColor(_backgroundColor);
  _maxBufferLength = 1024;

  _bufferText.setFont(font);
  _bufferText.setFillColor(sf::Color::White);

  _bufferHintText.setFont(font);
  _bufferHintText.setFillColor(_foregroundColor);

  _outputText.setFont(font);
  _outputText.setFillColor(_foregroundColor);

  _notifyText.setFont(font);
  _notifyText.setFillColor(_foregroundColor);
  _longestCommandName = 4; // help
  _notifySeconds = 5.0f;
  _notifyFadeSeconds = 1.0f;

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
  _notifyText.setCharacterSize(_charSize * 1.4f);

  _notifyAnchor = sf::Vector2f(margin, screenHeight - margin);
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
    if (_textBuffer.size() == 0)
    {
      // on first open
      _commandBuffer = "help";
      sendCommand();
    }
    else
    {
      updateBufferText();
      updateOutputText();
    }
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
  std::string helpMessage = "";
  if (_commandBuffer.size() != 0)
  {
    for(const auto& [command, help] : _commands)
    {
      if (command.rfind(_commandBuffer, 0) == 0)
      {
        _hintCommand = command;
        if (command.size() == _commandBuffer.size())
          helpMessage = "\t" + help;
        break;
      }
    }
  }
  _bufferHintText.setString(_prompt + _hintCommand + helpMessage);
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
    print(_commands, "  - ", _longestCommandName + 4);
  }
  else if (_commands.contains(command._name))
  {
    print("> " + _commandBuffer);
    Events::Console->publish(command._name, command._arg);
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

void Console::notify(const std::string& line)
{
  _notifyMessages.push_back(std::pair(line, _notifySeconds + _notifyFadeSeconds));
  print(line);
  Log::info(line);
}

void Console::print(const std::string& line)
{
  _textBuffer.push_back(line);
  if (_textBuffer.size() > _textBufferSize)
  {
    _textBuffer.erase(_textBuffer.begin());
  }
}

void Console::print(const std::vector<std::string>& lines, const std::string newlinePrefix)
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

void Console::print(const std::map<std::string, std::string>& dic, const std::string newlinePrefix, size_t secondColumnOffset)
{
  for (const auto& [first, second] : dic)
  {
    if (secondColumnOffset == 0)
    {
      _textBuffer.push_back(newlinePrefix + first + "\t\t" + second);
    }
    else
    {
      std::string padding(secondColumnOffset - first.size(), ' ');
      _textBuffer.push_back(newlinePrefix + first + padding + second);
    }
    if (_textBuffer.size() > _textBufferSize)
    {
      _textBuffer.erase(_textBuffer.begin());
    }
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

  auto it = std::remove_if(_notifyMessages.begin(), _notifyMessages.end(),
  [&](auto& notify)
  {
    notify.second -= deltaSeconds;
    return notify.second <= 0.0f;
  });

  _notifyMessages.erase(it, _notifyMessages.end());
}

void Console::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
  if (!_bIsOpen)
  {
    // notify text
    sf::Color color;
    sf::Vector2f pos = _notifyAnchor;
    sf::Text text = _notifyText;
    for (int i = 0; i < _notifyMessages.size(); i++)
    {
      pos.y = _notifyAnchor.y - (_notifyMessages.size() - i) * _notifyText.getCharacterSize();
      text.setPosition(pos);
      text.setString(_notifyMessages[i].first);
      color = _foregroundColor;
      if (_notifyMessages[i].second < _notifyFadeSeconds)
        color.a = 255 * _notifyMessages[i].second / _notifyFadeSeconds;
      text.setFillColor(color);
      rt.draw(text, states);
    }
    return;
  }
  rt.draw(_background, states);
  rt.draw(_outputText, states);
  rt.draw(_bufferHintText, states);
  rt.draw(_bufferText, states);
}

void Console::addCommand(const std::string& command, const std::string& help)
{
  if (_commands.contains(command))
  {
    Log::warn(Str::agg("Command: ", command ," already exists"));
  }
  else
  {
    _longestCommandName = std::max(_longestCommandName, command.size());
    _commands[command] = help;
  }
}
