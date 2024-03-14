#pragma once

#include <cstddef>

#include "Utils/EventSystem/EventSystem.hpp"

enum EGameEvent : size_t
{
  NA = 0,
  ACTIVATE_CELLS,
  ACTIVATE_CELLS_COMPLETE,
  CALC_NEIGHBORS,
  CALC_NEIGHBORS_COMPLETE
};

static EventSystem<EGameEvent> GameEvents;
