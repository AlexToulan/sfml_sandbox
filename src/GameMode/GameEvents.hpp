#pragma once

#include <cstddef>
#include <memory>
#include <mutex>

#include "Utils/EventSystem/EventSystem.hpp"

enum EGameEvent : size_t
{
  NA = 0,
  TOGGLE_PAUSE,
  ACTIVATE_CELLS,
  ACTIVATE_CELLS_COMPLETE,
  CALC_NEIGHBORS,
  CALC_NEIGHBORS_COMPLETE
};

namespace Events
{
  // TODO: Not a huge fan of this but it works for now. Possibility of overriding original instance
  // flushing all event bindings and leaving orphaned bindings in event listeners; nbd they are only
  // used for referance tracking and can be pruned.
  extern std::unique_ptr<EventSystem<EGameEvent>> Game;
}
// TODO: consider using this to ensure one instance.
// namespace GameEventSystem
// {
//   std::once_flag instance;
//   inline void initOnce()
//   {
//     std::call_once(instance, []()
//     {
//       GameEvents = std::make_unique<EventSystem<EGameEvent>>("GameEvents");
//     });
//   }
// }
