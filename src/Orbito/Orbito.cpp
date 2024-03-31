#include "GameMode/ConsoleEvents.hpp"
#include "Utils/Logging.hpp"
#include "Utils/MathUtils.hpp"
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
  _whitePile.setOutlineColor(sf::Color(255, 255, 255));
  _whitePile.setOutlineThickness(4.0f);
  _black.setFillColor(sf::Color(32, 32, 32));
  _blackPile.setFillColor(sf::Color(32, 32, 32));
  _blackPile.setOutlineColor(sf::Color(255, 255, 255));
  _blackPile.setOutlineThickness(0.0f);
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

sf::Vector2f Orbito::onResize(int screenX, int screenY)
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
  _cycleButton.setRadius(cellRadius * 0.6f);

  _white.setOrigin(_white.getRadius(), _white.getRadius());
  _black.setOrigin(_black.getRadius(), _black.getRadius());
  _cycleButton.setOrigin(_cycleButton.getRadius(), _cycleButton.getRadius());
  _cycleButton.setPosition(halfScreenPx.x, halfScreenPx.y);

  sf::Vector2f boardCellSize = sf::Vector2f(_white.getRadius() * 2.2f, _white.getRadius() * 2.2f);
  _boardSlots.fill(sf::RectangleShape(boardCellSize));
  int i = 0;
  for (auto& cell : _boardSlots)
  {
    sf::Vector2f pos = getCellPos(i++);
    cell.setSize(boardCellSize);
    cell.setOrigin(boardCellSize * 0.5f);
    cell.setPosition(pos);
    cell.setFillColor(sf::Color(222, 222, 222, 32));
    cell.setOutlineColor(sf::Color(255, 255, 255, 0));
    cell.setOutlineThickness(4.0f);
  }

  _whitePile.setRadius(cellRadius);
  _blackPile.setRadius(cellRadius);
  _whitePile.setPosition(16.0f, 16.0f);
  _blackPile.setPosition(screenX - _blackPile.getLocalBounds().width - 16.0f, 16.0f);

  return sf::Vector2f();
}

void Orbito::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::KeyReleased)
  {
    if (event.key.code == sf::Keyboard::Enter)
      Events::Console->publish("restart_game_mode");
  }
  if (_gameState != EGameState::WHITES_TURN && _gameState != EGameState::BLACKS_TURN)
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

  for (int i = 0; i < _boardCellStates.size(); i++)
  {
    sf::Vector2f pos = getCellPos(i);
    if (_bAnimatePieces)
    {
      float t = _animDelta /_animSeconds;
      pos = mu::lerp(pos, getCellPos(_cycleBoardIdx[i]), t * t);
    }
    if (_boardCellStates[i] == ECell::WHITE)
    {
      _white.setPosition(pos);
      window.draw(_white);
    }
    else if (_boardCellStates[i] == ECell::BLACK)
    {
      _black.setPosition(pos);
      window.draw(_black);
    }
  }
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
      setCellHighlight(i, true);
      _hoverCellIndex = i;
    }
    else
    {
      setCellHighlight(i, false);
    }
  }
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
  {
    _whitePile.setOutlineThickness(0.0f);
    _blackPile.setOutlineThickness(4.0f);
    _gameState = EGameState::BLACKS_TURN;
    Events::Console->publish<std::string>("notify", "Black's Turn");
  }
  else if (_gameState == EGameState::BLACKS_TURN)
  {
    _whitePile.setOutlineThickness(4.0f);
    _blackPile.setOutlineThickness(0.0f);
    _gameState = EGameState::WHITES_TURN;
    Events::Console->publish<std::string>("notify", "White's Turn");
  }

  winCheck();
}

int Orbito::winCheckLine(const std::array<int, _boardWidth>& lineIdx) const
{
  int sum = _boardCellStates[lineIdx[0]] + _boardCellStates[lineIdx[1]]
    + _boardCellStates[lineIdx[2]] + _boardCellStates[lineIdx[3]];

  if (sum == _boardWidth)
    return 1;
  if (sum == -_boardWidth)
    return -1;
  return 0;
}

void Orbito::winCheck()
{
  std::vector<int> winLines;
  for (const auto& lineIdx : _winRunIdx)
  {
    int check = winCheckLine(lineIdx);
    if (check != 0)
    {
      winLines.push_back(check);
      for (auto idx : lineIdx)
      {
        setCellHighlight(idx, true);
      }
    }
  }
  if (winLines.size() == 0)
  {
    // no winning lines and no empty spaces
    if (std::find(_boardCellStates.begin(), _boardCellStates.end(), 0) == _boardCellStates.end())
    {
      _gameState = EGameState::TIE;
      Events::Console->publish<std::string>("notify", "TIE!!!");
    }
  }
  else
  {
    int sumWinChecks = std::reduce(winLines.begin(), winLines.end());
    std::string message;
    if (sumWinChecks >= ECell::WHITE)
    {
      message = "White Wins!";
      _gameState = EGameState::WHITE_WINS;
    }
    else if (sumWinChecks <= ECell::BLACK)
    {
      message = "Black Wins!";
      _gameState = EGameState::BLACK_WINS;
    }
    else
    {
      message = "TIE!!!";
      _gameState = EGameState::TIE;
    }
    Events::Console->publish("notify", message);
  }
}

void Orbito::toggleAnimation(bool bOn)
{
  _bAnimatePieces = bOn;
  _animDelta = 0.0f;
}

void Orbito::setCellHighlight(int idx, bool bHighlight)
{
  sf::Color color = _boardSlots[idx].getOutlineColor();
  color.a = bHighlight ? 127 : 0;
  _boardSlots[idx].setOutlineColor(color);
}

