#pragma once

#include <cstddef>
#include <memory>
#include <mutex>

#include "Utils/EventSystem/EventSystem.hpp"

enum EGameEvent : size_t
{
  NA = 0,
  ACTIVATE_CELLS,
  ACTIVATE_CELLS_COMPLETE,
  CALC_NEIGHBORS,
  CALC_NEIGHBORS_COMPLETE
};

namespace Events
{
  extern std::unique_ptr<EventSystem<EGameEvent>> Game;
}
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
