#pragma once

#include <string>
#include <memory>
#include <mutex>

#include "Utils/EventSystem/EventSystem.hpp"

namespace Events
{
  // TODO: Not a huge fan of this but it works for now. Possibility of overriding original instance
  // flushing all event bindings and leaving orphaned bindings in event listeners; nbd they are only
  // used for referance tracking and can be pruned.
  extern std::unique_ptr<EventSystem<std::string>> Console;
}
// TODO: consider using this to ensure one instance.
// namespace ConsoleEventSystem
// {
//   std::once_flag instance;
//   inline void initOnce()
//   {
//     std::call_once(instance, []()
//     {
//       ConsoleEvents = std::make_unique<EventSystem<std::string>>("ConsoleEvents");
//     });
//   }
// }
