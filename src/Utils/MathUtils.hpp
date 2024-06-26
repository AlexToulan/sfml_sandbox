#pragma once

namespace mu
{
  template<class T>
  T wrap(const T& value, const T& min, const T& max)
  {
    if (value < min)
      return max;
    if (value > max)
      return min;
    return value;
  }

  template<class T>
  T lerp(const T& a, const T& b, float t)
  {
    return a + t * (b - a);
  }
}