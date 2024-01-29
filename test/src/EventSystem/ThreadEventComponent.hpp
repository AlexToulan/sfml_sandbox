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
    subscribe(EventType::REQ_DOUBLE_INTS, std::bind(&ThreadEventComponent::receiveNumbers, 
            this, std::placeholders::_1));

    if (pthread_create(&_thread, NULL, &_thread_bootstrap, (void*)this) != 0)
    {
      Log::error("failed to start thread");
      return false;
    }
    return true;
  }

  void stop()
  {
    pthread_join(_thread, nullptr);
  }

private:
  void receiveNumbers(const EventBase& event)
  {
    _dataProcessed = false;
    _inNumbers = unpack<std::vector<int>>(event);
  }

  bool run()
  {
    if (!_dataProcessed)
    {
      std::vector<int> doubled;
      for (size_t i = 0; i < _inNumbers.size(); i++)
      {
        doubled.push_back(_inNumbers[i] * 2);
      }
      EventComponent::publish(EventType::VECTOR_INT, Event<std::vector<int>>(doubled));
      _dataProcessed = true;
      return false;
    }
    return true;
  }

  bool _dataProcessed;
  std::vector<int> _inNumbers;

  pthread_t _thread;
  static void* _thread_bootstrap(void* s)
  {
    int exit_code = 0;
    while(static_cast<ThreadEventComponent*>(s)->run())
    {

    }
    pthread_exit(&exit_code);

    return nullptr;
  }
};
