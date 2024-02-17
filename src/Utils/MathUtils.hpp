#pragma once

namespace mu
{
  template<class T>
  T wrap(T value, T min, T max)
  {
    if (value < min)
      return max;
    if (value > max)
      return min;
    return value;
  }
}