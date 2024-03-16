#pragma once

#include "Utils/EventSystem/EventListener.hpp"
#include "GameMode/GameMode.hpp"
#include "SFML/System/Vector2.hpp"

#include <array>

class Orbito : public EventListener, public GameMode
{
public:
  Orbito();
  virtual ~Orbito();
  virtual void onStart();
  virtual void onResize(int screenX, int screenY);
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  enum EGameState : size_t
  {
    WHITES_TURN = 0,
    BLACKS_TURN,
    WHITE_WINS,
    BLACK_WINS,
    TIE
  };

  enum ECell : int
  {
    BLACK = -1,
    NONE = 0,
    WHITE = 1
  };

  static const size_t _boardWidth = 4;
  static const size_t _boardNumCells = _boardWidth * _boardWidth;

  sf::Vector2f getCellPos(int index) const;
  void processMouseClick(const sf::Vector2f& mousePos, bool bLeft);
  void processMouseMove(const sf::Vector2f& mousePos);
  void cycleCells();
  int winCheckLine(const std::array<int, _boardWidth>& lineIdx) const;
  void winCheck();
  void toggleAnimation(bool bOn = true);
  void setCellHighlight(int idx, bool bHighlight);
  sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t) const;

  // board
  EGameState _gameState;
  std::array<ECell, _boardNumCells> _boardCellStates;
  std::array<int, _boardNumCells> _cycleBoardIdx;
  std::array<std::array<int, _boardWidth>, _boardWidth * 2 + 2> _winRunIdx;
  sf::Transform _cellSize;
  sf::Vector2f _offSet;
  bool _bSelectionMade;

  // input
  bool _bSpacePressed;
  bool _bMouseLeft;
  sf::Vector2f _mousePos;

  // animation
  bool _bAnimatePieces;
  float _animDelta;
  float _animSeconds;

  // UI
  sf::CircleShape _whitePile;
  sf::CircleShape _blackPile;
  sf::CircleShape _cycleButton;
  std::array<sf::RectangleShape, _boardNumCells> _boardSlots;

  bool _bWhiteSelected;
  bool _bBlackSelected;
  bool _bCycleSelected;
  int _hoverCellIndex;

  // pieces
  sf::CircleShape _white;
  sf::CircleShape _black;
};
