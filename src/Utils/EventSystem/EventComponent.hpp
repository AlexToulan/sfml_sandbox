#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Event.hpp"

// A thread-safe event-component base-class that copies data to each subscriber.
// Publishers must construct data within event derived objects before publishing.
// Publishers can either destroy or recycle event derived objects after publishing.
class EventComponent
{
private:
  struct Delegate
  {
    Delegate(EventComponent* id, std::function<void(const EventBase&)>&& func)
    {
      _id = id;
      _func = std::move(func);
    }
    void operator()(const EventBase& e)
    {
      _func(e);
    }

    bool operator==(const Delegate& other) const
    {
      return _id == other._id;
    }

    bool operator!=(const Delegate& other) const
    {
      return _id != other._id;
    }

    bool isOwner(EventComponent* id) const
    {
      return id == _id;
    }
  private:
    EventComponent* _id;
    std::function<void(const EventBase&)> _func;
  };
public:
  // A delegate that can accept an Event and returns nothing
  // EventComponent pointer is only used to identify delegates belonging to the owner when unsubscribing
  // TODO: move typedef into separate space
  // typedef std::pair<EventComponent*, std::function<void(const EventBase&)>> Delegate;
  EventComponent();

  // Unsubscribes from all events
  virtual ~EventComponent();

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

protected:
  // Subscribe to a single event
  // func can only be a rvalue, like from std::bind
  void subscribe(const EventBase::Key& key, std::function<void(const EventBase&)>&& func);

  // Unsubscribe from all events
  void unsubscribe();

  // Unsubscribe from a single event
  void unsubscribe(const EventBase::Key& key);

  template<class T>
  static const T& unpack(const EventBase& event)
  {
    return static_cast<const Event<T>*>(&event)->data();
  }

private:
  // Used for locking the static delegate map
  static std::mutex _bindingMutex;
  // Needs to be static so other instances on other threads can access subscribers delegates
  static std::map<EventBase::Key, std::vector<Delegate>> _eventBindings;
};