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
  if (logFile.is_open())
    logFile << "[  INFO  ]  " << location.file_name() << ":" << location.line() << " " << "> " << message << std::endl;
  if (bShowInfo)
  {
    std::cout << "[  INFO  ]  " << blue()
      << location.file_name() << ":" << location.line() << std::endl << reset() << " > " << message << std::endl;
  }
}

void Log::warn(const std::string& message, const std::source_location& location)
{
  if (logFile.is_open())
    logFile << "[ WARNING ] " << location.file_name() << ":" << location.line() << " " << "> " << message << std::endl;
  if (bShowWarn)
  {
    std::cout << yellow() << "[ WARNING ] " << blue()
      << location.file_name() << ":" << location.line() << std::endl << reset() << " > " << message << std::endl;
  }
}

void Log::error(const std::string& message, const std::source_location& location)
{
  if (logFile.is_open())
    logFile << "[  ERROR  ] " << location.file_name() << ":" << location.line() << " " << "> " << message << std::endl;
  if (bShowError)
  {
    std::cerr << red() << "[  ERROR  ] " << blue()
      << location.file_name() << ":" << location.line() << std::endl << reset() << " > " << message << std::endl;
  }
}