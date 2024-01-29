#pragma once

#include "Events.hpp"
#include "EventTypes.hpp"
#include "Utils/EventSystem/EventComponent.hpp"

class SingleThreadEventComponent : public EventComponent
{
public:
  SingleThreadEventComponent() : EventComponent() {}
  virtual ~SingleThreadEventComponent(){}

  void setup()
  {
    subscribe(EventType::VECTOR_INT, &SingleThreadEventComponent::receivedNumbersEvent);
  }

  void setNumbers(const std::vector<int>& numbers)
  {
    _outNumbersEvent = Event<std::vector<int>>(numbers);
  }

  void send()
  {
    publish(EventType::REQ_DOUBLE_INTS, _outNumbersEvent);
  }

  void receivedNumbersEvent(const EventBase& event)
  {
    _inNumbers = copy<std::vector<int>>(event);
  }

  const std::vector<int>& getNumbers()
  {
    return _inNumbers;
  }

private:
  std::vector<int> _inNumbers;
  Event<std::vector<int>> _outNumbersEvent;
};