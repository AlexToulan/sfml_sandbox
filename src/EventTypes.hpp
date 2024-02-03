#pragma once

#include <cstddef>

enum EventType : size_t
{
  NA = 0,
  REQ_DOUBLE_INTS,
  VECTOR_INT,
  WORKER_START = 128,
  WORKER_END = 256
};
