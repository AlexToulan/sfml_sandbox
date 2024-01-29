#pragma once

#include "EventTypes.hpp"
#include "Utils/EventSystem/EventComponent.hpp"

class SingleThreadEventComponent : public EventComponent
{
public:
  SingleThreadEventComponent() : EventComponent() {}
  virtual ~SingleThreadEventComponent(){}

  void setup()
  {
  }

  void setOutNumbers(const std::vector<int>& numbers)
  {
    _outNumbersEvent = Event(numbers);
  }

  void send()
  {
    publish(EventType::REQ_DOUBLE_INTS, _outNumbersEvent);
  }

  void receivedNumbersEvent(const EventBase& event)
  {
    copy(event, _inNumbers);
  }

  void clearInNumbers()
  {
    _inNumbers.clear();
  }

  const std::vector<int>& getNumbers()
  {
    return _inNumbers;
  }

private:
  std::vector<int> _inNumbers;
  Event<std::vector<int>> _outNumbersEvent;
};
