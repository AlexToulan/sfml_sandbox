#pragma once

#include <string>
#include <source_location>

namespace Log
{
  const std::string red("\033[0;31m");
  const std::string green("\033[1;32m");
  const std::string yellow("\033[1;33m");
  const std::string blue("\033[1;34m");
  const std::string cyan("\033[0;36m");
  const std::string magenta("\033[0;35m");
  const std::string reset("\033[0m");

  void info(const std::string& message, const std::source_location& location = std::source_location::current());
  void warn(const std::string& message, const std::source_location& location = std::source_location::current());
  void error(const std::string& message, const std::source_location& location = std::source_location::current());
};
