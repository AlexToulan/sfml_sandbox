#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Utils/Timer.hpp"
#include "Utils/GameMode/GameMode.hpp"

class GameModeController
{
public:
  GameModeController();
  GameModeController(int screenWidth, int screenHeight, std::string windowTitle);
  ~GameModeController();
  void selectGameMode(const std::string& gameModeName);
  void addGameMode(std::unique_ptr<GameMode> gameMode);
  bool setup(unsigned int frameRate);
  void run();
  void teardown();
  void nextGameMode();
  void previousGameMode();

private:
  void switchGameMode(int direction);

  Timer _loopTimer;

  int _frameRate;
  sf::RenderWindow _window;
  size_t _currentGameModeIndex;
  std::vector<std::unique_ptr<GameMode>> _gameModes;

  bool _bNextKey;
  bool _bPreviousKey;
};