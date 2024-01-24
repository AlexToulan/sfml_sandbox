#include "GameOfLife/GameOfLifeGameMode.hpp"
#include "QuadTest/QuadTestGameMode.hpp"
#include "Utils/GameMode/GameModeController.hpp"
#include "Utils/Logging.hpp"
#include "Utils/ArgParse.hpp"

// more examples at https://github.com/morrisfranken/argparse/blob/master/examples/hello_world.cpp
struct Config : public argparse::Args
{
  bool& verbose = flag("v,verbose", "A flag to toggle verbose").set_default(false);
  std::string& log_file_path = arg("log_file", "log file path").set_default("log.txt");
  std::string& game_mode = arg("game_mode", "select the default game mode to start with").set_default("QuadTestGameMode");
};

int main(int argc, char* argv[])
{
  auto args = argparse::parse<Config>(argc, argv);
  if (args.verbose)
    args.print();

  GameModeController controller(1000, 1000, "Game Mode Test");

  // TODO: store game modes in a map by name and use cli args to select
  controller.addGameMode(std::make_unique<GameOfLifeGameMode>());
  controller.addGameMode(std::make_unique<QuadTestGameMode>());
  // controller.selectGameMode(args.game_mode);

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