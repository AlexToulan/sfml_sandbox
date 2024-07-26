#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>

#include "Utils/EventSystem/EventSystem.hpp"

enum EDebugEvents : size_t
{
  LINE = 0,
  BOX = 1
};

namespace Events
{
  // TODO: Not a huge fan of this but it works for now. Possibility of overriding original instance
  // flushing all event bindings and leaving orphaned bindings in event listeners; nbd they are only
  // used for reference tracking and can be pruned.
  typedef EventSystem<EDebugEvents> DebugEventSystem;
  extern std::unique_ptr<DebugEventSystem> Debug;
}
