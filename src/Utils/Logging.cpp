#include <filesystem>
#include <fstream>
#include <iostream>


#include "Logging.hpp"

bool Log::bShowInfo = false;
bool Log::bShowWarn = false;
bool Log::bShowError = true;
std::ofstream Log::logFile;

void Log::init(const std::string& filePath, bool showInfo, bool showWarn)
{
  logFile = std::ofstream(filePath, std::ofstream::trunc);
  bShowInfo = showInfo;
  bShowWarn = showWarn;
}

void Log::info(const std::string& message, const std::source_location& location)
{
  logFile << "[  INFO  ]  " << location.file_name() << ":" << location.line() << " " << "> " << message << std::endl;
  if (bShowInfo)
  {
    std::cout << "[  INFO  ]  " << blue()
      << location.file_name() << ":" << location.line() << " " << reset() << "> " << message << std::endl;
  }
}

void Log::warn(const std::string& message, const std::source_location& location)
{
  logFile << "[ WARNING ] " << location.file_name() << ":" << location.line() << " " << "> " << message << std::endl;
  if (bShowWarn)
  {
    std::cout << yellow() << "[ WARNING ] " << blue()
      << location.file_name() << ":" << location.line() << " " << reset() << "> " << message << std::endl;
  }
}

void Log::error(const std::string& message, const std::source_location& location)
{
  logFile << "[  ERROR  ] " << location.file_name() << ":" << location.line() << " " << "> " << message << std::endl;
  if (bShowError)
  {
    std::cout << red() << "[  ERROR  ] " << blue()
      << location.file_name() << ":" << location.line() << " " << reset() << "> " << message << std::endl;
  }
}