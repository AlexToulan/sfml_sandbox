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

  void _info(const char* file_name, const char* function_name, int line_num, const std::string& message);
  void _warn(const char* file_name, const char* function_name, int line_num, const std::string& message);
  void _error(const char* file_name, const char* function_name, int line_num, const std::string& message);
};

// TODO: upgrade this to use C++20 std::source_location. CMake is set to C++17
#define log_info(message) Log::_info(__FILE__, __FUNCTION__, __LINE__, (message))
#define log_warn(message) Log::_warn(__FILE__, __FUNCTION__, __LINE__, (message))
#define log_error(message) Log::_error(__FILE__, __FUNCTION__, __LINE__, (message))
