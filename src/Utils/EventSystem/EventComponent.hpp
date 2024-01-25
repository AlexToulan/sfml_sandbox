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
  static void publish(const Event::Key& key, const Event& event);

  // Enqueue a single event into subscriber queue(s)
  // Copies event data for each event component queue
  void queueEvent(const Event::Key& key, const Event& event);

  // Subscribe to a single event
  // func can only be a rvalue, like from std::bind
  void subscribe(const Event::Key& key, Delegate&& func);

  // Unsubscribe from all events
  // TODO: implementation still feels a bit clunky
  void unsubscribe();

  // Unsubscribe from a single event
  void unsubscribe(const Event::Key& key);

protected:
  // Fires events from the queue before clearing it and destroying the containing events
  void fireEvents();

private:
  // TODO: move statics into separate space
  static std::mutex _bindingMutex;
  static std::map<Event::Key, std::vector<EventComponent*>> _eventBindings;
  std::mutex _queueMutex;
  std::map<Event::Key, Event> _eventQueue;
  std::map<Event::Key, Delegate> _delegates;
};