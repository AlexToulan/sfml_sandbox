#pragma once

#include <cstddef>

class EventBase
{
public:
  typedef size_t Key;
  EventBase();
  ~EventBase();

  template<class T>
  const T& as() const
  {
    return *static_cast<const T*>(this);
  }
};
