#include <filesystem>
#include <fstream>

#include "Logging.hpp"

bool Log::bShowInfo = false;
bool Log::bShowWarn = false;
bool Log::bShowError = true;
std::mutex Log::_consoleMutex;
std::ofstream Log::logFile;

Log::Log(std::source_location location)
{
  _source = location;
}

void Log::init(const std::string& filePath, bool showInfo, bool showWarn)
{
  logFile = std::ofstream(filePath, std::ofstream::trunc);
  bShowInfo = showInfo;
  bShowWarn = showWarn;
}

void Log::debug(const std::string& message)
{
  if (bShowInfo)
  {
    std::scoped_lock lock(_consoleMutex);
    std::cout << cyan() << debugTag() << reset() << " > " << message << std::endl;
  }
}

void Log::info(const std::string& message)
{
  std::scoped_lock lock(_consoleMutex);
  if (logFile.is_open())
    logFile << infoTag() << _source.file_name() << ":" << _source.line() << " " << "> " << message << std::endl;
  if (bShowInfo)
  {
    std::cout << cyan() << infoTag() << blue()
      << _source.file_name() << ":" << _source.line() << std::endl << reset() << " > " << message << std::endl;
  }
}

void Log::warn(const std::string& message)
{
  std::scoped_lock lock(_consoleMutex);
  if (logFile.is_open())
    logFile << warnTag() << _source.file_name() << ":" << _source.line() << " " << "> " << message << std::endl;
  if (bShowWarn)
  {
    std::cout << yellow() << warnTag() << blue()
      << _source.file_name() << ":" << _source.line() << std::endl << reset() << " > " << message << std::endl;
  }
}

void Log::error(const std::string& message)
{
  std::scoped_lock lock(_consoleMutex);
  if (logFile.is_open())
    logFile << errorTag() << _source.file_name() << ":" << _source.line() << " " << "> " << message << std::endl;
  if (bShowError)
  {
    std::cerr << red() << errorTag() << blue()
      << _source.file_name() << ":" << _source.line() << std::endl << reset() << " > " << message << std::endl;
  }
}
