#pragma once

#include <format>
#include <string>
#include <sstream>

class Str
{
public:
  template<typename ...VArgs>
  static std::string agg(VArgs ...vargs)
  {
    // derp use std::format
    std::ostringstream ss;
    _agg(ss, vargs...);
    return ss.str();
  }

  template<typename ...VArgs>
  static std::string format(std::string& f, VArgs ...vargs)
  {
    return std::format(f, vargs...);
  }

private:
  template<typename T, typename ...VArgs>
  static void _agg(std::ostream& ss, T t, VArgs... vargs)
  {
    ss << t;
    _agg(ss, vargs...);
  }
  
  template<typename T>
  static void _agg(std::ostream& ss, T t)
  {
    ss << t;
  }
};