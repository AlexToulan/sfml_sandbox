#pragma once

#include <thread>

#include "Events.hpp"
#include "EventTypes.hpp"
#include "Utils/Logging.hpp"
#include "Utils/EventSystem/EventComponent.hpp"

class ThreadEventComponent : public EventComponent
{
public:
  ThreadEventComponent() : EventComponent() {}
  virtual ~ThreadEventComponent() {}

  bool start()
  {
    _dataProcessed = true;
    _isRunning = true;

    if (pthread_create(&_thread, NULL, &_thread_bootstrap, (void*)this) != 0)
    {
      Log::error("failed to start thread");
      return false;
    }
    return true;
  }

  void stop()
  {
    if (_isRunning)
    {
      _isRunning = false;
      pthread_join(_thread, nullptr);
    }
  }

  void receiveNumbers(const EventBase& event)
  {
    std::unique_lock<decltype(_numbersMutex)> lock(_numbersMutex);
    _dataProcessed = false;
    copy(event, _inNumbers);
  }

private:
  bool run()
  {
    if (!_dataProcessed)
    {
      std::vector<int> doubled;
      std::unique_lock<decltype(_numbersMutex)> lock(_numbersMutex);
      for (size_t i = 0; i < _inNumbers.size(); i++)
      {
        doubled.push_back(_inNumbers[i] * 2);
      }
      EventComponent::publish(EventType::VECTOR_INT, Event(doubled));
      _dataProcessed = true;
    }
    return _isRunning;
  }

  std::atomic<bool> _dataProcessed;
  std::atomic<bool> _isRunning;

  std::mutex _numbersMutex;
  std::vector<int> _inNumbers;

  pthread_t _thread;
  static void* _thread_bootstrap(void* s)
  {
    int exit_code = 0;
    ThreadEventComponent* self = static_cast<ThreadEventComponent*>(s);
    while(self->run())
    {

    }
    pthread_exit(&exit_code);

    return nullptr;
  }
};
