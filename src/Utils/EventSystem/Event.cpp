#include "Event.hpp"

Event::Event()
{
  set_type(0);
}

Event::Event(int type)
{
  set_type(type);
}

Event::~Event()
{
}

void Event::set_type(int type)
{
  _type = type;
}

int Event::get_type()
{
  return _type;
}