#pragma once

#include <memory>
#include <vector>

#include "Event.hpp"
#include "TDelegate.hpp"

class EventListener
{
public:
  typedef TDelegate<EventListener, const EventBase&> Delegate;

  EventListener() {}
  virtual ~EventListener() {}
  // Returns a delegate from the function signature parameter
  template<class T>
  std::shared_ptr<Delegate> bind(void(T::* func)(const EventBase&))
  {
    std::shared_ptr<Delegate> delegate = buildDelegate(func);
    
    _delegates.push_back(delegate);

    return delegate;
  }

protected:
  // Returns a reference to the template event data
  template<class T>
  static const T& unpack(const EventBase& eventBase)
  {
    return static_cast<const Event<T>*>(&eventBase)->data();
  }
  // Copies data within the template event to the outData parameter
  template<class T>
  static void copy(const EventBase& eventBase, T& outData)
  {
    outData = static_cast<const Event<T>*>(&eventBase)->data();
  }

  template<class T>
  std::shared_ptr<Delegate> buildDelegate(void(T::* func)(const EventBase&)) const
  {
    return std::make_shared<Delegate>(this, static_cast<void(EventListener::*)(const EventBase&)>(func));
  }

private:
  std::vector<std::shared_ptr<Delegate>> _delegates;
};
