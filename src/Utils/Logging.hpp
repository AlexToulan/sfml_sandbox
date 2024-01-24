#pragma once

#include <fstream>
#include <source_location>
#include <string>

class Log
{
public:
  Log() = delete;
  static void init(const std::string& filePath, bool showInfo, bool showWarn);
  static void info(const std::string& message, const std::source_location& location = std::source_location::current());
  static void warn(const std::string& message, const std::source_location& location = std::source_location::current());
  static void error(const std::string& message, const std::source_location& location = std::source_location::current());
  static bool bShowInfo;
  static bool bShowWarn;
  static bool bShowError;

private:
  static std::ofstream logFile;
  static const std::string red() { return std::string("\033[0;31m"); };
  static const std::string green() { return std::string("\033[1;32m"); };
  static const std::string yellow() { return std::string("\033[1;33m"); };
  static const std::string blue() { return std::string("\033[1;34m"); };
  static const std::string cyan() { return std::string("\033[0;36m"); };
  static const std::string magenta() { return std::string("\033[0;35m"); };
  static const std::string reset() { return std::string("\033[0m"); };
};
