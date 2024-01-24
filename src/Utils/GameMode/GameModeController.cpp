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

void GameModeController::selectGameMode(const std::string& gameModeName)
{
  size_t targetGameModeIndex = 0;
  for (auto& gameMode : _gameModes)
  {
    if (gameMode->getName() == gameModeName)
    {
      break;
    }
    targetGameModeIndex++;
  }
  switchGameMode(targetGameModeIndex - _currentGameModeIndex);
  if (_gameModes[_currentGameModeIndex]->getName() != gameModeName)
  {
    Log::error(gameModeName + " not found");
  }
}

void GameModeController::addGameMode(std::unique_ptr<GameMode> gameMode)
{
  Log::info("Adding " + gameMode->getName());
  _gameModes.push_back(std::move(gameMode));
  _bNextKey = false;
  _bPreviousKey = false;
}

bool GameModeController::setup(unsigned int frameRate)
{
  Log::info("Setting up game mode controller");
  _frameRate = frameRate;
  _window.setFramerateLimit(_frameRate);

  if (_gameModes.size() == 0)
  {
    Log::warn("No game modes to run");
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
    _gameModes[_currentGameModeIndex]->update(_loopTimer.deltaSeconds());
    _gameModes[_currentGameModeIndex]->render(_window);
    _window.display();
  }
}

void GameModeController::teardown()
{
  
}

void GameModeController::nextGameMode()
{
  Log::info("Next game mode");
  switchGameMode(1);
}

void GameModeController::previousGameMode()
{
  Log::info("Previous game mode");
  switchGameMode(-1);
}

void GameModeController::switchGameMode(int direction)
{
  if (_gameModes.size() < 2)
  {
    Log::warn("No game mode to switch to. Restarting game mode.");
  }


  Log::info("Switching to " + _gameModes[_currentGameModeIndex]->getName());
  _gameModes[_currentGameModeIndex]->onEnd();
  _currentGameModeIndex = (_currentGameModeIndex + direction) % _gameModes.size();
  _gameModes[_currentGameModeIndex]->onStart();
}