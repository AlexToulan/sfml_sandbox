#pragma once

#include <memory>
#include <string>
#include <vector>

#include "EventTypes.hpp"
#include "Utils/CliConfig.hpp"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/Timer.hpp"
#include "GameMode/GameMode.hpp"
#include "Console.hpp"

class GameModeController : public EventSystem
{
public:
  GameModeController(const sf::Font& consoleFont);
  GameModeController(const sf::Font& consoleFont, int screenWidth, int screenHeight, std::string windowTitle);
  virtual ~GameModeController();
  bool setup(unsigned int framesPerSecond, unsigned int updatesPerSecond);
  void addGameMode(std::unique_ptr<GameMode> gameMode);
  void selectGameMode(const std::string& gameModeName);
  void run();
  void teardown();
  void nextGameMode();
  void previousGameMode();

private:
  void processInput();
  void switchGameMode(int direction);

  // events
  void consoleCommand(const EventBase& event);

  Console _console;
  bool _bShouldClose;
  Timer _loopTimer;
  Timer _fpsTimer;
  int _frames;
  float _fpsSeconds;

  int _updatesPerSecond;
  sf::RenderWindow _window;
  size_t _currentGameModeIndex;
  std::vector<std::unique_ptr<GameMode>> _gameModes;

  // input
  bool _bNextKey;
  bool _bPreviousKey;
  bool _bConsoleOpenKey;
};
