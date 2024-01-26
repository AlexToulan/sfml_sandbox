#include "EventComponent.hpp"


std::mutex EventComponent::_bindingMutex;
std::map<Event::Key, std::vector<EventComponent*>> EventComponent::_eventBindings;

EventComponent::EventComponent()
{
}

EventComponent::~EventComponent()
{
  unsubscribe();
}

void EventComponent::subscribe(const Event::Key& key, Delegate&& func)
{
  _delegates.insert(std::pair(key, std::move(func)));
  std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
  auto bindingIt = _eventBindings.find(key);
  if (bindingIt == _eventBindings.end())
  {
    _eventBindings[key] = std::vector<EventComponent*>();
  }
  _eventBindings[key].push_back(this);
}

void EventComponent::unsubscribe()
{
  std::vector<Event::Key> keys;
  for (const auto& [key, func] : _delegates)
  {
    keys.push_back(key);
  }

  for (const auto& key : keys)
  {
    // TODO: might want to prevent constant locking and unlocking
    unsubscribe(key);
  }
}

void EventComponent::unsubscribe(const Event::Key& key)
{
  auto bindingIt = _eventBindings.find(key);
  if (bindingIt != _eventBindings.end())
  {
    std::unique_lock<decltype(_bindingMutex)> lock(_bindingMutex);
    auto trashStartIt = std::remove_if(bindingIt->second.begin(), bindingIt->second.end(),
      [&](EventComponent*& self)
      {
        return self == this;
      }
    );
    _eventBindings[key].erase(trashStartIt, _eventBindings[key].end());
  }
  _delegates.erase(key);
}

void EventComponent::queueEvent(const Event::Key& key, Event* event)
{
  std::unique_lock<decltype(_queueMutex)> lock(_queueMutex);
  _eventQueue.insert(std::pair(key, event));
}

void EventComponent::fireEvents()
{
  for (const auto& e : _eventQueue)
  {
    _delegates[e.first](*e.second);
    delete e.second;
  }
  _eventQueue.clear();
}