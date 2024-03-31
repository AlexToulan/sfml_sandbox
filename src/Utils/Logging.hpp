#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <tuple>

#include "Utils/Str.hpp"

class Log
{
public:
  Log(std::source_location location = std::source_location::current());
  Log(const Log& other) = delete;
  Log(Log&& other) = delete;
  Log operator=(const Log& other) = delete;

  static void init(const std::string& filePath, bool showInfo, bool showWarn);

  void debug(const std::string& message);
  template <typename ... VArgs>
  void debug(const std::string_view& format, VArgs... vargs)
  {
    debug(Str::format(format, vargs...));
  }
  
  void info(const std::string& message);
  template <typename ... VArgs>
  void info(const std::string_view& format, VArgs... vargs)
  {
    info(Str::format(format, vargs...));
  }

  void warn(const std::string& message);
  template <typename ... VArgs>
  void warn(const std::string_view& format, VArgs... vargs)
  {
    warn(Str::format(format, vargs...));
  }

  void error(const std::string& message);
  template <typename ... VArgs>
  void error(const std::string_view& format, VArgs... vargs)
  {
    error(Str::format(format, vargs...));
  }

  static bool bShowInfo;
  static bool bShowWarn;
  static bool bShowError;

private:
  std::source_location _source;
  static std::mutex _consoleMutex;
  static std::ofstream logFile;
  // category tags
  static const std::string debugTag() { return std::string("[  DEBUG  ] "); };
  static const std::string infoTag()  { return std::string("[  INFO   ] "); };
  static const std::string warnTag()  { return std::string("[ WARNING ] "); };
  static const std::string errorTag() { return std::string("[  ERROR  ] "); };
  // colors
  static const std::string red() { return std::string("\033[0;31m"); };
  static const std::string green() { return std::string("\033[1;32m"); };
  static const std::string yellow() { return std::string("\033[1;33m"); };
  static const std::string blue() { return std::string("\033[1;34m"); };
  static const std::string cyan() { return std::string("\033[0;36m"); };
  static const std::string magenta() { return std::string("\033[0;35m"); };
  static const std::string reset() { return std::string("\033[0m"); };
};
