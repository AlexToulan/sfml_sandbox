#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <vector>

#include "Event.hpp"
 
// A thread-safe event-component base-class that copies data to each subscriber.
// Publishers must construct data within event derived objects before publishing.
// Publishers can either destroy or recycle event derived objects after publishing.
class EventComponent
{
public:
  // A delegate that can accept an Event and returns nothing
  // TODO: move typedef into separate space
  typedef std::function<void(const Event&)> Delegate;

  EventComponent();

  // Unsubscribes from all events
  virtual ~EventComponent();

  // TODO: move statics into separate space
  // Copies event data for each event component queue
  template<class T>
  static void publish(const Event::Key& key, const T& event)
  {
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      // copy event data for event component each queue
      for (const auto& eventComponent : bindingIt->second)
      {
        // we need to copy the event data per listener immediately
        // events copies are deleted once fired in each listener instance
        eventComponent->queueEvent(key, static_cast<Event*>(new T(event)));
      }
    }
  }

  // Subscribe to a single event
  // func can only be a rvalue, like from std::bind
  void subscribe(const Event::Key& key, Delegate&& func);

  // Unsubscribe from all events
  // TODO: implementation still feels a bit clunky
  void unsubscribe();

  // Unsubscribe from a single event
  void unsubscribe(const Event::Key& key);

protected:
  // Enqueue a single event into subscriber queue(s)
  void queueEvent(const Event::Key& key, Event* event);

protected:
  // Fires events from the queue before clearing it and destroying the containing events
  void fireEvents();

private:
  // TODO: move statics into separate space
  static std::mutex _bindingMutex;
  static std::map<Event::Key, std::vector<EventComponent*>> _eventBindings;
  std::mutex _queueMutex;
  std::map<Event::Key, Event*> _eventQueue;
  std::map<Event::Key, Delegate> _delegates;
};