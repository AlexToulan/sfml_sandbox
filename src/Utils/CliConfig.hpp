#pragma once

#include "thirdparty/ArgParse.hpp"

// more examples at https://github.com/morrisfranken/argparse/blob/master/examples/hello_world.cpp
struct CliConfig : public argparse::Args
{
  bool& log_info = flag("li,log_info", "show info in the console").set_default(true);
  bool& log_warn = flag("lw,log_warn", "show warnings in the console").set_default(true);
  std::string& log_file_path = kwarg("lf,log_file", "log file path").set_default("log.txt");
  std::string& game_mode = kwarg("gm,game_mode", "select the default game mode to start with")
    .set_default("GameModeSelect");
  int& frames_per_second = kwarg("fps,frames_per_second", "frame rate used for graphics").set_default("60");
  int& updates_per_second = kwarg("ups,updates_per_second", "update rate used for all game modes").set_default("30");
};
