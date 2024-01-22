#include "Utils/GameMode/GameModeController.hpp"
#include "Utils/Logging.hpp"


GameModeController::GameModeController()
  : _window(sf::VideoMode(1000, 1000), "SFML Test")
{
  _currentGameModeIndex = 0;
}

GameModeController::GameModeController(int screenWidth, int screenHeight, std::string windowTitle)
  : _window(sf::VideoMode(screenWidth, screenHeight), windowTitle)
{
  _currentGameModeIndex = 0;
}

GameModeController::~GameModeController()
{

}

void GameModeController::addGameMode(std::unique_ptr<GameMode> gameMode)
{
  log_info("Adding " + gameMode->getName());
  _gameModes.push_back(std::move(gameMode));
  _bNextKey = false;
  _bPreviousKey = false;
}

bool GameModeController::setup(unsigned int frameRate)
{
  _frameRate = frameRate;
  _window.setFramerateLimit(_frameRate);

  if (_gameModes.size() == 0)
  {
    log_warn("No game modes to run");
    return false;
  }

  _gameModes[_currentGameModeIndex]->onStart();
  return true;
}

void GameModeController::run()
{
  while (_window.isOpen())
  {
    sf::Event event;
    while (_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed || event.KeyReleased && event.key.code == sf::Keyboard::Escape)
      {
        _gameModes[_currentGameModeIndex]->onEnd();
        _window.close();
        return;
      }
      _gameModes[_currentGameModeIndex]->processEvents(event);
    }

    // cycle game modes
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Period))
      _bNextKey = true;
    else
    {
      if (_bNextKey)
        nextGameMode();
      _bNextKey = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Comma))
      _bPreviousKey = true;
    else
    {
      if (_bPreviousKey)
        previousGameMode();
      _bPreviousKey = false;
    }

    _window.clear();
    _gameModes[_currentGameModeIndex]->update(_loopTimer.pollLoopSeconds());
    _gameModes[_currentGameModeIndex]->render(_window);
    _window.display();
  }
}

void GameModeController::teardown()
{
  
}

void GameModeController::nextGameMode()
{
  log_info("Next Game Mode");
  switchGameMode(1);
}

void GameModeController::previousGameMode()
{
  log_info("Previous Game Mode");
  switchGameMode(-1);
}

void GameModeController::switchGameMode(int direction)
{
  if (_gameModes.size() < 2)
  {
    log_warn("No game mode to switch to. Restarting game mode.");
  }

  if (direction > 1)
    direction = 1;
  if (direction < -1)
    direction = -1;

  _gameModes[_currentGameModeIndex]->onEnd();
  _currentGameModeIndex = (_currentGameModeIndex + direction) % _gameModes.size();
  _gameModes[_currentGameModeIndex]->onStart();
}