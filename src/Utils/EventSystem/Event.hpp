#pragma once

#include "EventBase.hpp"

template<class T>
class Event : public EventBase
{
public:
  Event() : EventBase(), _data()
  {
  }
  Event(const T& data) : EventBase(), _data(data)
  {
  }
  Event(const Event& other) : EventBase(), _data(other.data)
  {
  }

  const T& data() const
  {
    return _data;
  }

private:
  T _data;
};