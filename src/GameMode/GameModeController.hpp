#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Utils/Timer.hpp"
#include "GameMode/GameMode.hpp"

class GameModeController
{
public:
  GameModeController();
  GameModeController(int screenWidth, int screenHeight, std::string windowTitle);
  ~GameModeController();
  bool setup(unsigned int frameRate);
  void addGameMode(std::unique_ptr<GameMode> gameMode);
  void selectGameMode(const std::string& gameModeName);
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