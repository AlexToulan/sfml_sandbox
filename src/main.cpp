#include "GameOfLife/GameOfLife.hpp"
#include "QuadTest/QuadTest.hpp"
#include "Orbito/Orbito.hpp"
#include "GameMode/GameModeController.hpp"
#include "Utils/Logging.hpp"
#include "Utils/CliConfig.hpp"
#include <memory>

int main(int argc, char* argv[])
{
  int returnCode = 0;
  auto args = argparse::parse<CliConfig>(argc, argv);
  Log::init(args.log_file_path, args.log_info, args.log_warn);
  if (args.log_info)
    args.print();
  
  sf::Font consoleFont;
  if (!consoleFont.loadFromFile("resources/Fonts/UbuntuMono-Regular.ttf"))
  {
    Log::error("failed to load console font");
    return 2;
  }
  GameModeController controller(consoleFont, 1000, 1000, "Game Mode Test");

  controller.addGameMode(std::make_unique<QuadTest>());
  controller.addGameMode(std::make_unique<GameOfLife>());
  controller.addGameMode(std::make_unique<Orbito>());

  if (controller.setup(args.frames_per_second, args.updates_per_second))
  {
    controller.selectGameMode(args.game_mode);
    controller.run();
  }
  else
  {
    Log::error("GameModeController failed to setup.");
    returnCode = 1;
  }
  controller.teardown();
  return returnCode;
}
