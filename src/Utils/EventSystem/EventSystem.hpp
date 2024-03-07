#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Utils/Logging.hpp"
#include "TDelegate.hpp"
#include "Event.hpp"
#include "EventListener.hpp"

// A thread-safe event-component base-class that allows for publishing/subscribing of events.
// Publishers must construct data within Event template objects before publishing.
// Publishers can either destroy or recycle event derived objects after publishing.
template<class TKey>
class EventSystem
{
public:
  EventSystem()
  {
  }

  ~EventSystem()
  {
  }

  // Publishes an event on the calling thread.
  // Delegates are responsible for coping incoming data and locking destination containers.
  bool publish(const TKey& key)
  {
    EventBase placeholder;
    return publish(key, placeholder);
  }

  // Publishes an event on the calling thread.
  // Delegates are responsible for coping incoming data and locking destination containers.
  bool publish(const TKey& key, const EventBase& eventBase)
  {
    bool succ = true;
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      for (auto& del : _eventBindings[key])
      {
        if (del.unique())
        {
          Log::error("EventListener was destroyed without unsubscribing from events");
          succ = false;
        }
        else
        {
          del->exec(eventBase);
        }
      }
    }
    return succ;
  }

  // Subscribe to a single event
  bool subscribe(const TKey& key, const std::shared_ptr<EventListener::Delegate>& delegate_ptr)
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    // enforce unique keys
    if (_eventBindings.find(key) == _eventBindings.end())
    {
      _eventBindings[key] = std::vector<std::shared_ptr<EventListener::Delegate>>();
    }
    // prevent duplicate delegates under the same event key
    auto it = std::find_if(_eventBindings[key].begin(), _eventBindings[key].end(),
    [&](const std::shared_ptr<EventListener::Delegate> del_ptr)
    {
      return (*del_ptr) == (*delegate_ptr);
    });
    // if (std::find(_eventBindings[key].begin(), _eventBindings[key].end(), delegate) == _eventBindings[key].end())
    if (it == _eventBindings[key].end())
    {
      _eventBindings[key].emplace_back(delegate_ptr);
    }
    else
    {
      // users can still subscribe with duplicate delegate instances if under different event types
      Log::warn("key: [" + std::to_string(key) + "] attempted to subscribe more than once with the same delegate instance");
      return false;
    }
    return true;
  }

  // Unsubscribe from all events
  void unsubscribe(const EventListener* listener)
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    for (auto& [key, delegates] : _eventBindings)
    {
      auto eraseStartIt = std::remove_if(delegates.begin(), delegates.end(),
        [&](std::shared_ptr<EventListener::Delegate>& del)
        {
          return del->isOwner(listener);
        }
      );
      delegates.erase(eraseStartIt, delegates.end());
    }
  }

  // Unsubscribe from a single event
  void unsubscribe(const TKey& key, const EventListener* listener)
  {
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
      auto eraseStartIt = std::remove_if(bindingIt->second.begin(), bindingIt->second.end(),
        [&](std::shared_ptr<EventListener::Delegate>& del)
        {
          return del->isOwner(listener);
        }
      );
      _eventBindings[key].erase(eraseStartIt, _eventBindings[key].end());
    }
  }

  // clears all subscriptions from the event system
  void flushSubscribers()
  {
    _eventBindings.clear();
  }

private:
  std::mutex _bindingMutex;
  std::map<TKey, std::vector<std::shared_ptr<EventListener::Delegate>>> _eventBindings;
};
