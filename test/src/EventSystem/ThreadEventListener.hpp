#pragma once

#include <thread>

#include "TestEvents.hpp"
#include "Utils/Logging.hpp"
#include "Utils/EventSystem/EventListener.hpp"

class ThreadEventListener : public EventListener
{
public:
  ThreadEventListener() : EventListener(), _thread()
  {
    _isRunning = false;
    _dataProcessed = false;
  }

  virtual ~ThreadEventListener()
  {
    stop();
  }

  void start()
  {
    _isRunning = true;
    _thread = std::thread(&ThreadEventListener::run, this);
  }

  void stop()
  {
    if (_isRunning)
    {
      _isRunning = false;
      _thread.join();
    }
  }

  void receiveNumbers(const EventBase& event)
  {
    std::unique_lock<decltype(_numbersMutex)> lock(_numbersMutex);
    _dataProcessed = false;
    copy(event, _inNumbers);
  }

private:
  void run()
  {
    while (_isRunning)
    {
      if (!_dataProcessed)
      {
        std::vector<int> doubled;
        std::unique_lock<decltype(_numbersMutex)> lock(_numbersMutex);
        for (size_t i = 0; i < _inNumbers.size(); i++)
        {
          doubled.push_back(_inNumbers[i] * 2);
        }
        TestEvents.publish(EventType::VECTOR_INT, Event(doubled));
        _dataProcessed = true;
      }
    }
  }

  std::atomic<bool> _isRunning;
  std::atomic<bool> _dataProcessed;

  std::mutex _numbersMutex;
  std::vector<int> _inNumbers;

  std::thread _thread;
};