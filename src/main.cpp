#include "GameOfLife/GameOfLifeGameMode.hpp"
#include "QuadTest/QuadTestGameMode.hpp"
#include "Utils/GameMode/GameModeController.hpp"
#include "Utils/Logging.hpp"
#include "Utils/ArgParse.hpp"

// more examples at https://github.com/morrisfranken/argparse/blob/master/examples/hello_world.cpp
struct Config : public argparse::Args
{
  bool& log_info = flag("li,log_info", "show info in the console").set_default(true);
  bool& log_warn = flag("lw,log_warn", "show warnings in the console").set_default(true);
  std::string& log_file_path = kwarg("lf,log_file", "log file path").set_default("log.txt");
  std::string& game_mode = kwarg("gm,game_mode", "select the default game mode to start with").set_default("QuadTestGameMode");
};

int main(int argc, char* argv[])
{
  auto args = argparse::parse<Config>(argc, argv);
  if (args.log_info)
    args.print();
  
  Log::init(args.log_file_path, args.log_info, args.log_warn);

  GameModeController controller(1000, 1000, "Game Mode Test");

  // TODO: store game modes in a map by name and use cli args to select
  controller.addGameMode(std::make_unique<QuadTestGameMode>());
  controller.addGameMode(std::make_unique<GameOfLifeGameMode>());

  if (controller.setup(60))
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