#include <filesystem>
#include <fstream>
#include <iostream>

#include "Logging.hpp"

#define DEBUG_LOG
#define PATH_LINK_VISIBLE
// #define FUNCTION_NAME_VISIBLE
namespace Log
{
  void info(const std::string& message, const std::source_location& location)
  {
    #ifdef DEBUG_LOG
      std::cout << "[  INFO  ]  " << blue
      #ifdef PATH_LINK_VISIBLE
        << location.file_name() << ":" << location.line() << " "
      #endif
      #ifdef FUNCTION_NAME_VISIBLE
        << location.function_name() << "()"
      #endif
        << reset << "> " << message << std::endl;
    #endif
  }

    void warn(const std::string& message, const std::source_location& location)
  {
    #ifdef DEBUG_LOG
    std::cout << yellow << "[ WARNING ] " << blue
    #ifdef PATH_LINK_VISIBLE
      << location.file_name() << ":" << location.line() << " "
    #endif
    #ifdef FUNCTION_NAME_VISIBLE
      << location.function_name() << "()"
    #endif
      << reset << "> " << message << std::endl;
    #endif
  }

  void error(const std::string& message, const std::source_location& location)
  {
    std::cout << red << "[  ERROR  ] " << blue
    #ifdef PATH_LINK_VISIBLE
      << location.file_name() << ":" << location.line() << " "
    #endif
    #ifdef FUNCTION_NAME_VISIBLE
      << location.function_name() << "()"
    #endif
      << reset << "> " << message << std::endl;
  }
}