#include "EventComponent.hpp"


std::mutex EventComponent::_bindingMutex;
std::map<EventBase::Key, std::vector<EventComponent::Delegate>> EventComponent::_eventBindings;

EventComponent::EventComponent()
{
}

EventComponent::~EventComponent()
{
  unsubscribe();
}

void EventComponent::unsubscribe()
{
  std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
  for (auto& [key, delegates] : _eventBindings)
  {
    auto eraseStartIt = std::remove_if(delegates.begin(), delegates.end(),
      [&](const Delegate& del)
      {
        return del.isOwner(this);
      }
    );
    delegates.erase(eraseStartIt, delegates.end());
  }
}

void EventComponent::unsubscribe(const EventBase::Key& key)
{
  auto bindingIt = _eventBindings.find(key);
  if (bindingIt != _eventBindings.end())
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    auto eraseStartIt = std::remove_if(bindingIt->second.begin(), bindingIt->second.end(),
      [&](Delegate& del)
      {
        return del.isOwner(this);
      }
    );
    _eventBindings[key].erase(eraseStartIt, _eventBindings[key].end());
  }
}
