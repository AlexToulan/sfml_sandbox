#pragma once

#include <string>
#include <sstream>

class Str
{
public:
  /// @brief Concatenates a parameter pack with a string stream
  /// @tparam ...VArgs 
  /// @param ...vargs 
  /// @return The stringified parameter pack
  template<typename ...VArgs>
  static std::string agg(VArgs ...vargs)
  {
    std::ostringstream ss;
    _agg(ss, vargs...);
    return ss.str();
  }

  /// @brief A simple version of std::format. Does not support argument ids or format specifications.
  /// @tparam ...VArgs Variadic template arguments
  /// @param format A string view to format output
  /// @param ...vargs Parameter pack
  /// @return A formatted string
  template<typename ...VArgs>
  static std::string format(std::string_view format, VArgs ...vargs)
  {
    std::string out;
    _format(out, format, vargs...);
    // append suffix
    out += format.substr(0, format.size());
    return out;
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

  /// @brief Replacement field key. (TODO: Take an int to support arguments ids; -1 to disable)
  /// @return The replacement field string
  static const std::string field()
  {
    return "{}";
  }

  /// @brief Recursively parses leading elemets of the given parameter pack. Escapes if there are no more replacement fields.
  /// @tparam T A single type from the variadic template arguments
  /// @param out The resulting string
  /// @param format A string view to format output
  /// @param t A single element from the parameter pack provided
  template<typename T, typename ...VArgs>
  static void _format(std::string& out, std::string_view& format, T t, VArgs... vargs)
  {
    if (_format(out, format, t))
    {
      _format(out, format, vargs...);
    }
  }

  /// @brief Adds the suffix of the format buffer and one argument from the callers parameter pack
  /// @tparam T A single type from the variadic template arguments
  /// @param out The resulting string
  /// @param format A string view to format output
  /// @param t A single element from the parameter pack provided
  /// @return False if there are no more replacement fields, true otherwise.
  template<typename T>
  static bool _format(std::string& out, std::string_view& format, T t)
  {
    auto nextFieldPos = format.find_first_of(field());
    if (nextFieldPos == format.npos)
    {
      return false;
    }
    else
    {
      out += format.substr(0, nextFieldPos);
      format.remove_prefix(nextFieldPos + field().length());
      std::ostringstream ss;
      ss << t;
      out += ss.str();
    }
    return true;
  }
};
