#pragma once

#include <cmath>
#include <limits>
#include <SFML/System/Vector2.hpp>

#include "Utils/MathUtils.hpp"


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

template <typename T>
bool operator <(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
  return left.x < right.x && left.y < right.y;
}

template <typename T>
bool operator >(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
  return left.x > right.x && left.y > right.y;
}

template <typename T>
bool operator <=(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
  return left.x <= right.x && left.y <= right.y;
}

template <typename T>
bool operator >=(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
  return left.x >= right.x && left.y >= right.y;
}

namespace vu
{
  const float epsilon = std::numeric_limits<float>().epsilon();
  const float float_min = std::numeric_limits<float>().min();
  const float float_max = std::numeric_limits<float>().max();
  const float to_degrees = 180.0f / 3.1415926f;
  const float to_radians = 3.1415926f / 180.0f;

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
    auto theta = float(rand() % 360) * to_radians;
    return sf::Vector2f(cosf(theta), sinf(theta));
  }

  static sf::Vector2f normalized(const sf::Vector2f& vec, float len = 1.0f)
  {
    float invLength = len / (length(vec) + vu::epsilon);
    return vec * invLength;
  }

  static std::pair<float, sf::Vector2f> decompose(const sf::Vector2f& vec)
  {
    float l = length(vec);
    return { l, sf::Vector2f(vec / (l + vu::epsilon)) };
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

namespace ray
{
  struct Hit
  {
    Hit(bool c = false, sf::Vector2f p = sf::Vector2f())
    {
      collision = c;
      point = p;
    }
    bool collision;
    sf::Vector2f point;
  };

  static Hit cast(Hit& outHit, const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d)
  {
    outHit.collision = false;
    float a1 = b.y - a.y;
    float b1 = a.x - b.x;
 
    float a2 = d.y - c.y;
    float b2 = c.x - d.x;
    float determinant = a1 * b2 - a2 * b1;

    if (determinant != 0.0f)
      return outHit;


    float c1 = a1 * a.x + b1 * a.y;
    float c2 = a2 * c.x + b2 * c.y;
    outHit.point = sf::Vector2f((b2 * c1 - b1 * c2) / determinant, (a1 * c2 - a2 * c1) / determinant);
    if (a.y > b.y)
      outHit.collision = outHit.point <= a && outHit.point >= b;
    else
      outHit.collision = outHit.point <= b && outHit.point >= a;
    return outHit;
  }

  static Hit cast(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d)
  {
    Hit hit;
    cast(hit, a, b, c, d);
    return hit;
  }
}
