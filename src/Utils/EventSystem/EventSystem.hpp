#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Utils/Logging.hpp"
#include "TDelegate.hpp"
#include "EventListener.hpp"

/// @brief
/// A thread-safe event-component base-class that allows for publishing/subscribing of events.
/// Publishers must construct data within Event template objects before publishing.
/// Publishers can either destroy or recycle event derived objects after publishing.

template<class TKey>
class EventSystem final
{
public:
  EventSystem()
  {
  }

  ~EventSystem()
  {
  }

  /// Publishes an event on the calling thread.
  /// @param key The event key to publish under. </param>
  /// @return True: if there are no stale event bindings.
  template<class P>
  bool publish(const TKey& key) const
  {
    P placeholder;
    return publish(key, placeholder);
  }

  /// Publishes an event on the calling thread.
  /// @param key The event key to publish under.
  /// @param data The data to pass to the listener.
  /// @return True: if there are no stale event bindings.
  template<typename P>
  bool publish(const TKey& key, const P& data) const
  {
    bool succ = true;
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      for (auto& del : _eventBindings.at(key))
      {
        if (del.unique())
        {
          Log::error("EventListener destroyed without unsubscribing from event system bindings.");
          succ = false;
        }
        else
        {
          // TODO: enforce type-safety
          del->template execAs<EventListener, P>(data);
        }
      }
    }
    return succ;
  }

  /// Subscribe to a single event
  /// @param key The event key to listen for.
  /// @param delegate_ptr Pointer to shared generic delegate.
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

  ///
  /// @brief Unsubscribe from all events for this EventListener
  /// 
  /// @param listener The EventListener that's unsubscribing
  ///
  void unsubscribe(EventListener* listener)
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    for (auto& [key, delegates] : _eventBindings)
    {
      auto eraseStartIt = std::remove_if(delegates.begin(), delegates.end(),
        [&](std::shared_ptr<EventListener::Delegate>& del)
        {
          return del->isCaller(listener);
        }
      );
      delegates.erase(eraseStartIt, delegates.end());
    }
    listener->pruneBindings();
  }

  ///
  /// @brief Unsubscribe from a single event type for this EventListener
  /// 
  /// @param key Key to unsubscribe from
  /// @param listener EventListener that's unsubscribing
  ///
  void unsubscribe(const TKey& key, EventListener* listener)
  {
    auto bindingIt = _eventBindings.find(key);
    if (bindingIt != _eventBindings.end())
    {
      std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
      auto eraseStartIt = std::remove_if(bindingIt->second.begin(), bindingIt->second.end(),
        [&](std::shared_ptr<EventListener::Delegate>& del)
        {
          return del->isCaller(listener);
        }
      );
      _eventBindings[key].erase(eraseStartIt, _eventBindings[key].end());
    }
    listener->pruneBindings();
  }

  // removes delegates with dangling references
  void pruneBindings()
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    for (auto& [key, delegates] : _eventBindings)
    {
      auto eraseStartIt = std::remove_if(delegates.begin(), delegates.end(),
        [&](std::shared_ptr<EventListener::Delegate>& del)
        {
          // if the event system is the only owner of this delegate, it is a dangling reference
          return del.unique();
        }
      );
      delegates.erase(eraseStartIt, delegates.end());
    }
  }

private:
  std::mutex _bindingMutex;
  std::map<TKey, std::vector<std::shared_ptr<EventListener::Delegate>>> _eventBindings;
};
