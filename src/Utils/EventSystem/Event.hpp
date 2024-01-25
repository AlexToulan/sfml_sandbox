#pragma once
#include <cstddef>
class Event
{
public:
  Event();
  ~Event();
  typedef size_t Key;

  template<class T>
  const T& as() const
  {
    return *static_cast<T*>(this);
  }

private:
};
