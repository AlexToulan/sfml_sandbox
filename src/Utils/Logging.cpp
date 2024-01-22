#include <filesystem>
#include <fstream>
#include <iostream>

#include "Logging.hpp"

#define DEBUG_LOG
#define PATH_LINK_VISIBLE
// #define FUNCTION_NAME_VISIBLE
namespace Log
{
  void _info(const char* file_name, const char* function_name, int line_num, const std::string& message)
  {
    #ifdef DEBUG_LOG
      std::cout << "[  INFO  ]  " << blue
      #ifdef PATH_LINK_VISIBLE
        << file_name << ":" << line_num << " "
      #endif
      #ifdef FUNCTION_NAME_VISIBLE
        << function_name << "()"
      #endif
        << reset << " > " << message << std::endl;
    #endif
  }

  void _warn(const char* file_name, const char* function_name, int line_num, const std::string& message)
  {
    #ifdef DEBUG_LOG
      std::cout << yellow << "[ WARNING ] " << blue
      #ifdef PATH_LINK_VISIBLE
        << file_name << ":" << line_num << " "
      #endif
      #ifdef FUNCTION_NAME_VISIBLE
        << function_name << "()"
      #endif
        << reset << " > " << message << std::endl;
    #endif
  }

  void _error(const char* file_name, const char* function_name, int line_num, const std::string& message)
  {
    std::cout << red << "[  ERROR  ] " << blue
    #ifdef PATH_LINK_VISIBLE
      << file_name << ":" << line_num << " "
    #endif
    #ifdef FUNCTION_NAME_VISIBLE
      << function_name << "()"
    #endif
      << reset << " > " << message << std::endl;
  }
}