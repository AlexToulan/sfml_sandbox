#pragma once

#include <memory>

#include "Utils/EventSystem/EventSystem.hpp"
#include "TestType.hpp"

namespace Events
{
  extern std::unique_ptr<EventSystem<ETestType>> Test;
}
