#include <string>

#include "ConsoleEvents.hpp"
#include "GameEvents.hpp"
#include "GameModeController.hpp"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/Logging.hpp"

std::unique_ptr<EventSystem<EGameEvent>> Events::Game;
std::unique_ptr<EventSystem<std::string>> Events::Console;

GameModeController::GameModeController(const sf::Font& consoleFont)
  : EventListener()
  , _window(sf::VideoMode(1000, 1000), "SFML Test")
  , _console(consoleFont, 1000, 1000)
{
  _currentGameModeIndex = 0;
  _frames = 0;
  _bShouldClose = false;
}

GameModeController::GameModeController(const sf::Font& consoleFont, int screenWidth, int screenHeight, std::string windowTitle)
  : EventListener()
  , _window(sf::VideoMode(screenWidth, screenHeight), windowTitle)
  , _console(consoleFont, screenWidth, screenHeight, 1)
{
  _currentGameModeIndex = 0;
  _frames = 0;
  _bShouldClose = false;
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

bool GameModeController::setup(unsigned int framesPerSecond, unsigned int updatesPerSecond)
{
  Log::info("Setting up game mode controller");
  Events::Game = std::make_unique<EventSystem<EGameEvent>>("GameEvents");
  Events::Console = std::make_unique<EventSystem<std::string>>("ConsoleEvents");

  _console.addCommand("exit", "quits the application");
  _console.addCommand("quit", "quits the application");
  _console.addCommand("restart_game_mode", "restarts the current game mode");
  _console.addCommand("frames_per_second", "[int arg] sets the GPU framerate of the application");
  _console.addCommand("updates_per_second", "[int arg] sets the CPU framerate of the application");
  Events::Console->bind("exit", this, &GameModeController::exit);
  Events::Console->bind("quit", this, &GameModeController::exit);
  Events::Console->bind("restart_game_mode", this, &GameModeController::restartGameMode);
  Events::Console->bind("frames_per_second", this, &GameModeController::setFramesPerSecond);

  _window.setFramerateLimit(framesPerSecond);
  _updatesPerSecond = updatesPerSecond;
  _bConsoleOpenKey = false;
  _bShouldClose = false;

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
  _fpsTimer.start();
  while (_window.isOpen())
  {
    sf::Event event;
    while (_window.pollEvent(event))
    {
      if (_bShouldClose || event.type == sf::Event::Closed || event.KeyReleased && event.key.code == sf::Keyboard::Escape)
      {
        _gameModes[_currentGameModeIndex]->onEnd();
        _window.close();
        return;
      }
      if (_console.isOpen())
      {
        _console.processEvents(event);
      }
      else
      {
        _gameModes[_currentGameModeIndex]->processEvents(event);
      }
    }

    processInput();

    _frames++;
    if (_fpsTimer.pollSeconds() > 5.0f)
    {
      Log::info("FPS: " + std::to_string(_frames / 5));
      _fpsTimer.start();
      _frames = 0;
    }

    _window.clear();
    float ds = _loopTimer.deltaSeconds();
    _console.update(ds);
    _gameModes[_currentGameModeIndex]->update(ds);
    _gameModes[_currentGameModeIndex]->render(_window);
    _window.draw(_console);
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

void GameModeController::processInput()
{
  bool grave = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Grave);
  if (grave && !_bConsoleOpenKey)
  {
    _console.toggle();
    _bConsoleOpenKey = true;
  }
  _bConsoleOpenKey = grave;

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
}

void GameModeController::switchGameMode(int direction)
{
  if (_gameModes.size() < 2)
  {
    Log::warn("No game mode to switch to. Restarting game mode.");
  }

  _gameModes[_currentGameModeIndex]->onEnd();
  // EventSystem::flushSubscribers(); // allows event types per game mode
  _currentGameModeIndex = (_currentGameModeIndex + direction) % _gameModes.size();
  Log::info("Switching to " + _gameModes[_currentGameModeIndex]->getName());
  _gameModes[_currentGameModeIndex]->onStart();
}

void GameModeController::exit()
{
  _bShouldClose = true;
  Log::info("bye!");
}

void GameModeController::restartGameMode()
{
  _gameModes[_currentGameModeIndex]->onEnd();
  _gameModes[_currentGameModeIndex]->onStart();
}

void GameModeController::setFramesPerSecond(const std::string& fps)
{
    int framesPerSecond = std::stoi(fps);
    if (framesPerSecond > 0)
    {
      Log::info("frames_per_second: " + std::to_string(framesPerSecond));
      _window.setFramerateLimit((unsigned long)framesPerSecond);
    }
    else
    {
      Log::warn("\tinvalid argument: \"" + fps + "\"");
    }
}
