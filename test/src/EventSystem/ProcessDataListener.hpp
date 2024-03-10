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
    _outNumbers = numbers;
  }

  void send()
  {
    TestEvents.publish(EventType::REQ_DOUBLE_INTS, _outNumbers);
  }

  void receivedNumbers(const std::vector<int>& nums)
  {
    _inNumbers = nums;
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
  std::vector<int> _outNumbers;
};
