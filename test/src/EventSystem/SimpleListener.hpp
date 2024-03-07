#pragma once

#include "EventTypes.hpp"
#include "Utils/EventSystem/EventListener.hpp"

class SimpleListener : public EventListener
{
public:
  SimpleListener() : EventListener(), _number(0) {}
  virtual ~SimpleListener(){}

  void receivedNumbersEvent(const EventBase& event)
  {
    _number += unpack<int>(event);
  }

  const int getNumber() const
  {
    return _number;
  }

private:
  int _number;
};
