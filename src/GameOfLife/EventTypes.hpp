#pragma once

#include <cstddef>

enum EventType : size_t
{
  NA = 0,
  TOGGLE_PAUSE,
  ACTIVATE_CELLS,
  ACTIVATE_CELLS_COMPLETE,
  CALC_NEIGHBORS,
  CALC_NEIGHBORS_COMPLETE
};
