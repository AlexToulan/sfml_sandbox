#include "GameOfLife/GameOfLifeGameMode.hpp"
#include "QuadTest/QuadTestGameMode.hpp"
#include "Utils/GameMode/GameModeController.hpp"
#include "Utils/Logging.hpp"

int main()
{
  GameModeController controller(1000, 1000, "Game Mode Test");

  // TODO: store game modes in a map by name and use cli args to select
  controller.addGameMode(std::make_unique<QuadTestGameMode>());
  controller.addGameMode(std::make_unique<GameOfLifeGameMode>());

  if (controller.setup(60))
  {
    controller.run();
  }
  else
  {
    Log::error("GameModeController failed to setup.");
  }
  controller.teardown();
  return 0;
}