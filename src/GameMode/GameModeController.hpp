#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Utils/CliConfig.hpp"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/EventSystem/EventListener.hpp"
#include "Utils/Timer.hpp"
#include "GameMode/GameMode.hpp"
#include "Console.hpp"

class GameModeController : public EventListener
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
  bool _bShouldClose;
  Timer _loopTimer;
  Timer _fpsTimer;
  int _frames;


  float _currentSecPerUpdate;
  float _secPerUpdate;
  int _updatesPerSecond;
  int _framesPerSecond;

  sf::Vector2u _originalScreenSize;
  sf::RenderWindow _window;
  size_t _currentGameModeIndex;
  std::vector<std::unique_ptr<GameMode>> _gameModes;

  // input
  bool _bNextKey;
  bool _bPreviousKey;
  bool _bConsoleOpenKey;
};
