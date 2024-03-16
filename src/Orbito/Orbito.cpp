#include "GameMode/ConsoleEvents.hpp"
#include "Utils/Logging.hpp"
#include "Orbito.hpp"

#include <numeric>
#include <source_location>
#include <vector>

Orbito::Orbito()
  : EventListener()
  , GameMode(std::source_location::current().file_name())
{
}

Orbito::~Orbito()
{
}

void Orbito::onStart()
{
  _clearColor = sf::Color(64, 64, 64);
  _boardCellStates.fill(ECell::NONE);
  _white.setFillColor(sf::Color(192, 192, 192));
  _whitePile.setFillColor(sf::Color(192, 192, 192));
  _black.setFillColor(sf::Color(32, 32, 32));
  _blackPile.setFillColor(sf::Color(32, 32, 32));
  _hover.setFillColor(sf::Color(0, 0, 0, 0));
  _hover.setOutlineThickness(4.0f);
  _hover.setOutlineColor(sf::Color(32, 64, 192, 127));
  _cycleButton.setFillColor(sf::Color(222, 222, 222, 64));
  _gameState = EGameState::WHITES_TURN;
  _bSelectionMade = false;
  _bSpacePressed = false;
  toggleAnimation(false);
  _animSeconds = 0.6f;

  _cycleBoardIdx = {
    4,  0,  1,  2,
    8,  9,  5,  3,
    12, 10, 6,  7,
    13, 14, 15, 11
  };
  _winRunIdx = {{
    // --
    { 0,  1,  2,  3  },
    { 4,  5,  6,  7  },
    { 8,  9,  10, 11 },
    { 12, 13, 14, 15 },
    // |
    { 0,  4,  8,  12 },
    { 1,  5,  9,  13 },
    { 2,  6,  10, 14 },
    { 3,  7,  11, 15 },
    // X
    { 0,  5,  10, 15 },
    { 3,  6,  9,  12 }
  }};
}

void Orbito::onResize(int screenX, int screenY)
{
  _screenSize = sf::Vector2i(screenX, screenY);
  sf::Vector2i halfScreenPx = _screenSize / 2;
  float cellSizePx = std::min(screenX, screenY) / float(_boardWidth + 2);
  float halfBoardSizePx = (_boardWidth - 1.0f) * 0.5f * cellSizePx;
  _cellSize = sf::Transform();
  _cellSize.scale(cellSizePx, cellSizePx);
  _offSet = sf::Vector2f(
    halfScreenPx.x - halfBoardSizePx,
    halfScreenPx.y - halfBoardSizePx);

  float cellRadius = cellSizePx * 0.3f;
  float halfPieceRad = cellRadius * 0.5f;

  _white.setRadius(cellRadius);
  _black.setRadius(cellRadius);
  _hover.setRadius(cellRadius * 1.05f);
  _cycleButton.setRadius(cellRadius * 0.5f);

  _white.setOrigin(_white.getRadius(), _white.getRadius());
  _black.setOrigin(_black.getRadius(), _black.getRadius());
  _hover.setOrigin(_hover.getRadius(), _hover.getRadius());
  _cycleButton.setOrigin(_cycleButton.getRadius(), _cycleButton.getRadius());

  _hover.setPosition(-_hover.getRadius() * 2.0f, -_hover.getRadius() * 2.0f);
  _cycleButton.setPosition(halfScreenPx.x, halfScreenPx.y);

  sf::Vector2f boardCellSize = sf::Vector2f(_hover.getRadius() * 2.0f, _hover.getRadius() * 2.0f);
  _boardSlots.fill(sf::RectangleShape(boardCellSize));
  int i = 0;
  for (auto& cell : _boardSlots)
  {
    sf::Vector2f pos = getCellPos(i);
    i++;
    cell.setOrigin(_hover.getRadius(), _hover.getRadius());
    cell.setPosition(pos);
    cell.setFillColor(sf::Color(0, 0, 0, 0));
    cell.setFillColor(sf::Color(222, 222, 222, 32));
  }

  _whitePile.setRadius(cellRadius);
  _blackPile.setRadius(cellRadius);
  _whitePile.setPosition(16.0f, 16.0f);
  _blackPile.setPosition(screenX - _blackPile.getLocalBounds().width - 16.0f, 16.0f);
}

void Orbito::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::KeyReleased)
  {
    if (event.key.code == sf::Keyboard::Enter)
      Events::Console->publish("restart_game_mode");
  }
  if (_gameState >= EGameState::WHITE_WINS)
  {
    return;
  }

  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
      processMouseClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), true);
  }
  else if (event.type == sf::Event::MouseButtonReleased)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
      processMouseClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), false);
  }
  else if (event.type == sf::Event::MouseMoved)
  {
    processMouseMove(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    bool bSpacePressed = event.key.code == sf::Keyboard::Space;
    if (bSpacePressed && !_bSpacePressed)
      toggleAnimation();
    _bSpacePressed = bSpacePressed;
  }
  else if (event.type == sf::Event::KeyReleased)
  {
    bool bSpaceReleased = event.key.code == sf::Keyboard::Space;
    if (bSpaceReleased && _bSpacePressed)
    {
      toggleAnimation(false);
      cycleCells();
    }
    _bSpacePressed = !bSpaceReleased;
  }
}

void Orbito::update(float ds)
{
  if (_bAnimatePieces)
  {
    if (_animDelta >= _animSeconds)
    {
      _animDelta =_animSeconds;
    }
    else
    {
      _animDelta += ds;
    }
  }
}

void Orbito::render(sf::RenderWindow& window)
{
  window.draw(_whitePile);
  window.draw(_blackPile);
  window.draw(_cycleButton);
  for (const auto& cell : _boardSlots)
    window.draw(cell);
  int i = 0;
  for (const auto& state : _boardCellStates)
  {
    sf::Vector2f pos = getCellPos(i);
    if (_bAnimatePieces)
    {
      float t = _animDelta /_animSeconds;
      pos = lerp(pos, getCellPos(_cycleBoardIdx[i]), t * t * 0.7f);
    }
    i++;
    if (state == ECell::WHITE)
    {
      _white.setPosition(pos);
      window.draw(_white);
    }
    else if (state == ECell::BLACK)
    {
      _black.setPosition(pos);
      window.draw(_black);
    }
  }
  window.draw(_hover);
  if (_bWhiteSelected)
  {
    _white.setPosition(_mousePos);
    window.draw(_white);
  }
  if (_bBlackSelected)
  {
    _black.setPosition(_mousePos);
    window.draw(_black);
  }
}

void Orbito::onEnd()
{
}

sf::Vector2f Orbito::getCellPos(int index) const
{
  return _offSet + _cellSize * sf::Vector2f(index % _boardWidth, index / _boardWidth);
}

void Orbito::processMouseClick(const sf::Vector2f& mousePos, bool bLeft)
{
  _mousePos = mousePos;

  if (bLeft)
  {
    if (_bSelectionMade)
    {
      _bCycleSelected = _cycleButton.getGlobalBounds().contains(mousePos);
      if (_bCycleSelected)
        toggleAnimation();
    }
    else
    {
      _bWhiteSelected = _whitePile.getGlobalBounds().contains(mousePos) && _gameState == EGameState::WHITES_TURN;
      _bBlackSelected = _blackPile.getGlobalBounds().contains(mousePos) && _gameState == EGameState::BLACKS_TURN;
    }
  }
  else
  {
    if (_bCycleSelected && _cycleButton.getGlobalBounds().contains(mousePos))
    {
      cycleCells();
    }
    if (_hoverCellIndex >= 0)
    {
      if (_bWhiteSelected)
        _boardCellStates[_hoverCellIndex] = ECell::WHITE;
      if (_bBlackSelected)
        _boardCellStates[_hoverCellIndex] = ECell::BLACK;
      _bSelectionMade = _bWhiteSelected || _bBlackSelected;
    }
    _bWhiteSelected = false;
    _bBlackSelected = false;

    toggleAnimation(false);
  }

  _bMouseLeft = bLeft;
}

void Orbito::processMouseMove(const sf::Vector2f& mousePos)
{
  _mousePos = mousePos;
  
  _hoverCellIndex = -1;
  for (int i = 0; i < _boardSlots.size(); i++)
  {
    if (_boardSlots[i].getGlobalBounds().contains(mousePos) && _boardCellStates[i] == ECell::NONE)
    {
      _hover.setPosition(_boardSlots[i].getPosition());
      _hoverCellIndex = i;
      return;
    }
  }

  _hover.setPosition(-_hover.getRadius() * 2.0f, -_hover.getRadius() * 2.0f);
}

void Orbito::cycleCells()
{
  if (!_bSelectionMade)
    return;

  _bSelectionMade = false;
  std::array<ECell, _boardNumCells> temp = _boardCellStates;
  for (int i = 0; i < _boardNumCells; i++)
  {
    _boardCellStates[_cycleBoardIdx[i]] = temp[i];
  }

  if (_gameState == EGameState::WHITES_TURN)
    _gameState = EGameState::BLACKS_TURN;
  else if (_gameState == EGameState::BLACKS_TURN)
    _gameState = EGameState::WHITES_TURN;
    
  // win check
  std::vector<int> winLines;
  for (const auto& line : _winRunIdx)
  {
    int check = winCheckLine(line);
    if (check != 0)
    {
      winLines.push_back(check);
    }
  }
  if (winLines.size() > 0)
  {
    int sumWinChecks = std::reduce(winLines.begin(), winLines.end());
    if (sumWinChecks >= ECell::WHITE)
    {
      Log::info("White Wins!");
      _gameState = EGameState::WHITE_WINS;
    }
    else if (sumWinChecks <= ECell::BLACK)
    {
      Log::info("Black Wins!");
      _gameState = EGameState::BLACK_WINS;
    }
    else
    {
      Log::info("!!!TIE!!!");
      _gameState = EGameState::TIE;
    }
  }
}

int Orbito::winCheckLine(const std::array<int, _boardWidth>& line) const
{
  int sum = _boardCellStates[line[0]] + _boardCellStates[line[1]]
    + _boardCellStates[line[2]] + _boardCellStates[line[3]];
  if (sum == _boardWidth)
    return 1;
  if (sum == -_boardWidth)
    return -1;
  return 0;
}

void Orbito::toggleAnimation(bool bOn)
{
  _bAnimatePieces = bOn;
  _animDelta = 0.0f;
}

sf::Vector2f Orbito::lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t) const
{
  return a + t * (b - a);
}
