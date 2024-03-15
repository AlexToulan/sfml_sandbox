#pragma once

#include <string>
#include <sstream>

class Str
{
public:
  template<typename ...VArgs>
  static std::string agg(VArgs ...vargs)
  {
    std::ostringstream ss;
    _agg(ss, vargs...);
    return ss.str();
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