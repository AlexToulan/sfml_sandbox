#pragma once

#include <cmath>
#include <limits>
#include <SFML/System/Vector2.hpp>

#include "Utils/MathUtils.hpp"
#include "Logging.hpp"


// sf::Vector2 operators
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

  static float dot(const sf::Vector2f& aStart, const sf::Vector2f& aEnd)
  {
    return aStart.x * aEnd.x + aStart.y * aEnd.y;
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

  // static sf::Vector2f lerp(const sf::Vector2f& start, sf::Vector2f& end, float t)
  // {
  //   return (end - start) * t + start;
  // }

  template <typename R, typename T>
  sf::Vector2<R> cast(const sf::Vector2<T>& in)
  {
    return sf::Vector2<R>((R)in.x, (R)in.y);
  }

  struct Hit
  {
    Hit(bool bStart = false, sf::Vector2f p = sf::Vector2f())
    {
      collision = bStart;
      point = p;
    }
    bool collision;
    float det;
    sf::Vector2f point;
  };

  static void rayCast(vu::Hit& outHit, const sf::Vector2f& aStart, const sf::Vector2f& aEnd, const sf::Vector2f& bStart, const sf::Vector2f& bEnd)
  {
    outHit.point = sf::Vector2f();
    float a1 = aEnd.y - aStart.y;
    float b1 = aStart.x - aEnd.x;

    float a2 = bEnd.y - bStart.y;
    float b2 = bStart.x - bEnd.x;
    outHit.det = a1 * b2 - a2 * b1;

    outHit.collision = outHit.det != 0.0f;
    if (!outHit.collision)
      return;

    float c1 = a1 * aStart.x + b1 * aStart.y;
    float c2 = a2 * bStart.x + b2 * bStart.y;
    outHit.point = sf::Vector2f((b2 * c1 - b1 * c2) / outHit.det, (a1 * c2 - a2 * c1) / outHit.det);
  }

  static vu::Hit rayCast(const sf::Vector2f& aStart, const sf::Vector2f& aEnd, const sf::Vector2f& bStart, const sf::Vector2f& bEnd)
  {
    vu::Hit hit;
    cast(hit, aStart, aEnd, bStart, bEnd);
    return hit;
  }

  static void lineIntersect(vu::Hit& outHit, const sf::Vector2f& aStart, const sf::Vector2f& aEnd, const sf::Vector2f& bStart, const sf::Vector2f& bEnd)
  {
    outHit.collision = false;
    outHit.point = sf::Vector2f();
    float a1 = aEnd.y - aStart.y;
    float b1 = aStart.x - aEnd.x;

    float a2 = bEnd.y - bStart.y;
    float b2 = bStart.x - bEnd.x;
    outHit.det = a1 * b2 - a2 * b1;

    if (outHit.det == 0.0f)
      return;

    float c1 = a1 * aStart.x + b1 * aStart.y;
    float c2 = a2 * bStart.x + b2 * bStart.y;
    outHit.point = sf::Vector2f((b2 * c1 - b1 * c2) / outHit.det, (a1 * c2 - a2 * c1) / outHit.det);
    if (aStart.y > aEnd.y)
      outHit.collision = outHit.point <= aStart && outHit.point >= aEnd;
    else
      outHit.collision = outHit.point <= aEnd && outHit.point >= aStart;
  }

  static vu::Hit lineIntersect(const sf::Vector2f& aStart, const sf::Vector2f& aEnd, const sf::Vector2f& bStart, const sf::Vector2f& bEnd)
  {
    vu::Hit hit;
    lineIntersect(hit, aStart, aEnd, bStart, bEnd);
    return hit;
  }
}
