#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "TDelegate.hpp"

class EventListener
{
public:
  typedef TDelegate<EventListener, void*> Delegate;

  EventListener() {}
  virtual ~EventListener() {}
  // Returns a delegate from the function signature parameter
  template<class T, class P>
  std::shared_ptr<Delegate> bind(void(T::* func)(P))
  {
    std::shared_ptr<Delegate> delegate = buildDelegate(func);
    _delegates.push_back(delegate);
    return delegate;
  }

  void pruneBindings()
  {
    auto eraseStartIt = std::remove_if(_delegates.begin(), _delegates.end(),
      [&](std::shared_ptr<EventListener::Delegate>& del)
      {
        // if the event system is the only owner of this delegate, it is a dangling reference
        return del.unique();
      }
    );
    _delegates.erase(eraseStartIt, _delegates.end());
  }

private:
  template<class T, class P>
  std::shared_ptr<Delegate> buildDelegate(void(T::* func)(P)) const
  {
    return std::make_shared<Delegate>(this, reinterpret_cast<void(EventListener::*)(void*)>(func));
  }
  std::vector<std::shared_ptr<Delegate>> _delegates;
};
