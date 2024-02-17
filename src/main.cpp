#include "GameOfLife/GameOfLifeGameMode.hpp"
#include "QuadTest/QuadTestGameMode.hpp"
#include "GameMode/GameModeController.hpp"
#include "Utils/Logging.hpp"
#include "CliConfig.hpp"

int main(int argc, char* argv[])
{
  auto args = argparse::parse<CliConfig>(argc, argv);
  if (args.log_info)
    args.print();
  
  Log::init(args.log_file_path, args.log_info, args.log_warn);
  GameModeController controller(1000, 1000, "Game Mode Test");

  controller.addGameMode(std::make_unique<QuadTestGameMode>());
  controller.addGameMode(std::make_unique<GameOfLifeGameMode>());

  if (controller.setup(args.frame_rate))
  {
    controller.selectGameMode(args.game_mode);
    controller.run();
  }
  else
  {
    Log::error("GameModeController failed to setup.");
  }
  controller.teardown();
  return 0;
}