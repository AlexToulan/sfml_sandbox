#include "GameMode/GameModeController.hpp"
#include "GameMode/GameModeSelect.hpp"
#include "Games/GameOfLife/GameOfLife.hpp"
#include "Games/Orbito/Orbito.hpp"
#include "Games/Boids/Boids.hpp"
#include "Games/QuadTest/QuadTest.hpp"
#include "Games/Pathfinding/Pathfinding.hpp"

#include "Utils/Logging.hpp"
#include "Utils/CliConfig.hpp"
#include "Utils/Platform.hpp"

#include <memory>

std::string consoleFontPath = "resources/Fonts/UbuntuMono-Regular.ttf";
std::string menuFontPath = "resources/Fonts/Ubuntu-Bold.ttf";

bool loadFont(std::string fontName, sf::Font& outFont)
{
  std::string fontPath = Str::format("resources/Fonts/{}.ttf", fontName);
  if (!outFont.loadFromFile(fontPath))
  {
    Log().error("failed to load \"{}\" from path \"{}\"", fontName, fontPath);
    return false;
  }
  return true;
}

int main(int argc, char* argv[])
{
  Platform::config();
  int returnCode = 0;
  auto args = argparse::parse<CliConfig>(argc, argv);
  Log::init(args.log_file_path, args.log_info, args.log_warn);
  if (args.log_info)
    args.print();
  
  sf::Font consoleFont;
  sf::Font menuFont;
  if (!loadFont("UbuntuMono-Regular", consoleFont) || !loadFont("Ubuntu-Bold", menuFont))
  {
    return 2;
  }
  sf::Vector2u screenSize = sf::Vector2u(1000, 1000);
  GameModeController controller(consoleFont, screenSize.x, screenSize.y, "SFML Sandbox");

  controller.addGameMode(std::make_unique<GameModeSelect>(menuFont, screenSize));
  // controller.addGameMode(std::make_unique<QuadTest>(screenSize));
  controller.addGameMode(std::make_unique<GameOfLife>(screenSize));
  controller.addGameMode(std::make_unique<Orbito>(screenSize));
  controller.addGameMode(std::make_unique<Boids>(sf::Vector2u(1600, 1000)));
  controller.addGameMode(std::make_unique<Pathfinding>(sf::Vector2u(1600, 1000)));

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
