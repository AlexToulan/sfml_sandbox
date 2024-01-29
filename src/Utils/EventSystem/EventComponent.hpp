#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Delegate.hpp"
#include "Event.hpp"

// A thread-safe event-component base-class that copies data to each subscriber.
// Publishers must construct data within event derived objects before publishing.
// Publishers can either destroy or recycle event derived objects after publishing.
class EventComponent
{
public:
  // EventComponent pointer is only used to identify delegates belonging to the owner when unsubscribing
  // TODO: move typedef into separate space
  // typedef std::pair<EventComponent*, std::function<void(const EventBase&)>> Delegate;
  EventComponent();

  // Unsubscribes from all events
  virtual ~EventComponent();

  // Publishes an event on the calling thread.
  // Delegate implementations are responsible for coping incoming data and locking destination containers.
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
  // Delegate implementations are responsible for coping incoming data and locking destination containers.
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
  template<class T>
  static const T& copy(const EventBase& event)
  {
    return static_cast<const Event<T>*>(&event)->data();
  }

private:
  // Used for locking the static delegate map
  static std::mutex _bindingMutex;
  // Needs to be static so other instances on other threads can access subscribers delegates
  static std::map<EventBase::Key, std::vector<Delegate>> _eventBindings;
};