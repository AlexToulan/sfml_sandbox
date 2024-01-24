#pragma once

#include <chrono>

class Timer
{
public:
  Timer();
  ~Timer();
  void start();
  void stop();
  float getSeconds();
  float pollSeconds();

private:
  typedef std::chrono::high_resolution_clock hs_clock;
  hs_clock::time_point _start;
  hs_clock::time_point _end;
};