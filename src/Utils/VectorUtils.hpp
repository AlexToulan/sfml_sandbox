#pragma once

#include <cmath>
#include <limits>
#include <SFML/System/Vector2.hpp>

namespace vu
{
  static float toDegrees()
  {
    static float _toDegrees = 180.0f / 3.1415926f;
    return _toDegrees;
  }

  static float toRadians()
  {
    static float _toRadians = 3.1415926f / 180.0f;
    return _toRadians;
  }

  static float dot(const sf::Vector2f& a, const sf::Vector2f& b)
  {
    return a.x * b.x + a.y * b.y;
  }

  static float lengthSquared(const sf::Vector2f& vec)
  {
    return vec.x * vec.x + vec.y * vec.y;
  }

  static float length(const sf::Vector2f& vec)
  {
    return sqrt(lengthSquared(vec));
  }

  static sf::Vector2f randomDir()
  {
    auto theta = float(rand() % 360) * toRadians();
    return sf::Vector2f(cosf(theta), sinf(theta));
  }

  static sf::Vector2f normalized(const sf::Vector2f& vec)
  {
    return vec / (length(vec) + __FLT_EPSILON__);
  }

  static std::pair<float, sf::Vector2f> decompose(const sf::Vector2f& vec)
  {
    float l = length(vec);
    return { l, sf::Vector2f(vec / (l + __FLT_EPSILON__)) };
  }

  // static sf::Vector2f lerp(const sf::Vector2f& a, sf::Vector2f& b, float t)
  // {
  //   return (b - a) * t + a;
  // }

  template <typename R, typename T>
  sf::Vector2<R> cast(const sf::Vector2<T>& in)
  {
    return sf::Vector2<R>((R)in.x, (R)in.y);
  }
}

// operators
template <typename T>
sf::Vector2<T> operator *(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
  return sf::Vector2<T>(left.x * right.x, left.y * right.y);
}

template <typename T>
sf::Vector2<T> operator /(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
  return sf::Vector2<T>(left.x / right.x, left.y / right.y);
}
