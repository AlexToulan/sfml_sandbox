#include "GameMode/GameModeController.hpp"
#include "Games/GameOfLife/GameOfLife.hpp"
#include "Games/Orbito/Orbito.hpp"
#include "Games/Pathfinding/Pathfinding.hpp"
#include "Games/QuadTest/QuadTest.hpp"

#include "Utils/Logging.hpp"
#include "Utils/CliConfig.hpp"
#include "Utils/Platform.hpp"

#include <memory>

std::string consoleFontPath = "resources/Fonts/UbuntuMono-Regular.ttf";

int main(int argc, char* argv[])
{
  Platform::config();
  int returnCode = 0;
  auto args = argparse::parse<CliConfig>(argc, argv);
  Log::init(args.log_file_path, args.log_info, args.log_warn);
  if (args.log_info)
    args.print();
  
  sf::Font consoleFont;
  if (!consoleFont.loadFromFile(consoleFontPath))
  {
    Log().error("failed to load console font \"{}\"", consoleFontPath);
    return 2;
  }
  GameModeController controller(consoleFont, 1000, 1000, "Game Mode Test");

  controller.addGameMode(std::make_unique<QuadTest>());
  controller.addGameMode(std::make_unique<GameOfLife>());
  controller.addGameMode(std::make_unique<Orbito>());
  controller.addGameMode(std::make_unique<Pathfinding>());

  if (controller.setup(args.frames_per_second, args.updates_per_second))
  {
    controller.selectGameMode(args.game_mode);
    controller.run();
  }
  else
  {
    Log().error("GameModeController failed to setup.");
    returnCode = 1;
  }
  controller.teardown();
  return returnCode;
}
