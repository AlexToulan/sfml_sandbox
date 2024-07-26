#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Console.hpp"
#include "GameMode/GameMode.hpp"
#include "UI/HoverInfo.hpp"
#include "UI/DebugDraw.hpp"
#include "Utils/CliConfig.hpp"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/EventSystem/EventListener.hpp"
#include "Utils/Timer.hpp"

class GameModeController : public EventListener
{
public:
  GameModeController(const sf::Font& consoleFont, const sf::Font& hoverInfoFont);
  GameModeController(const sf::Font& consoleFont, const sf::Font& hoverInfoFont, int screenWidth, int screenHeight, std::string windowTitle);
  virtual ~GameModeController();
  bool setup(unsigned int framesPerSecond, unsigned int updatesPerSecond);
  void addGameMode(std::unique_ptr<GameMode> gameMode);
  void selectGameMode(const std::string& gameModeName);
  void run();
  void teardown();
  void nextGameMode();
  void previousGameMode();

private:
  void toggleConsole();
  void processInput();
  void switchGameMode(int direction);
  void resize(sf::Vector2u screenSize);
  void logFps();

  // events
  void exit();
  void restartGameMode();
  void setFramesPerSecond(const std::string& fps);
  void setUpdatesPerSecond(const std::string& ups);

  Console _console;
  DebugDraw _debugDraw;
  HoverInfo _hoverInfo;
  bool _bShouldClose;
  Timer _loopTimer;
  Timer _fpsTimer;
  int _frames;


  float _currentSecPerUpdate;
  float _secPerUpdate;
  int _updatesPerSecond;
  int _framesPerSecond;

  sf::RenderWindow _window;
  size_t _currentGameModeIndex;
  std::vector<std::unique_ptr<GameMode>> _gameModes;

  // input
  bool _bNextKey;
  bool _bPreviousKey;
  bool _bConsoleOpenKey;
};
