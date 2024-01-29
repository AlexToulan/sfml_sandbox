#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Delegate.hpp"
#include "Event.hpp"

// A thread-safe event-component base-class that allows for publishing/subscribing of events.
// Publishers must construct data within Event template objects before publishing.
// Publishers can either destroy or recycle event derived objects after publishing.
class EventComponent
{
public:
  EventComponent();

  // Unsubscribes from all events
  virtual ~EventComponent();

  // Publishes an event on the calling thread.
  // Delegates are responsible for coping incoming data and locking destination containers.
  static void publish(const EventBase::Key& key)
  {
    EventBase placeholder;
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      for (auto& del : _eventBindings[key])
      {
        del(placeholder);
      }
    }
  }

  // Publishes an event on the calling thread.
  // Delegates are responsible for coping incoming data and locking destination containers.
  template<class T>
  static void publish(const EventBase::Key& key, const Event<T>& event)
  {
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      for (auto& del : _eventBindings[key])
      {
        del(event);
      }
    }
  }

  // Subscribe to a single event
  template<class T>
  void subscribe(const EventBase::Key& key, void(T::* func)(const EventBase&))
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    auto delIt = _eventBindings.find(key);
    if (delIt == _eventBindings.end())
    {
      _eventBindings[key] = std::vector<Delegate>();
    }
    _eventBindings[key].emplace_back(this, static_cast<void(EventComponent::*)(const EventBase&)>(func));
  }

  // Unsubscribe from all events
  void unsubscribe();

  // Unsubscribe from a single event
  void unsubscribe(const EventBase::Key& key);

protected:
  typedef TDelegate<EventComponent, EventBase> Delegate;
  // Returns a reference to the template event data
  template<class T>
  static const T& unpack(const EventBase& event)
  {
    return static_cast<const Event<T>*>(&event)->data();
  }
  // Copies data within the template event to the outData parameter
  template<class T>
  static void copy(const EventBase& event, T& outData)
  {
    outData = static_cast<const Event<T>*>(&event)->data();
  }

private:
  // Used for locking the static delegate map
  static std::mutex _bindingMutex;
  // Needs to be static so other instances on other threads can access subscribers delegates
  static std::map<EventBase::Key, std::vector<Delegate>> _eventBindings;
};
