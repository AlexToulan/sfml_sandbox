#pragma once

#include "TestEvents.hpp"
#include "Utils/EventSystem/EventListener.hpp"

class ProcessDataListener : public EventListener
{
public:
  ProcessDataListener() : EventListener() {}
  virtual ~ProcessDataListener(){}

  void setup()
  {
  }

  void setOutNumbers(const std::vector<int>& numbers)
  {
    _outNumbersEvent = Event(numbers);
  }

  void send()
  {
    TestEvents.publish(EventType::REQ_DOUBLE_INTS, _outNumbersEvent);
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
