#pragma once

class Event
{
public:
  Event();
  Event(int type);
  ~Event();
  void set_type(int type);
  int get_type();

private:
  int _type;
};
