#pragma once

#include <chrono>

class Timer
{
public:
  Timer();
  ~Timer();
  void reset();
  void start();
  void stop();
  float getSeconds();
  float pollSeconds();
  float deltaSeconds();
  std::chrono::microseconds getMicroSecDuration();

private:
  std::chrono::high_resolution_clock::time_point _start;
  std::chrono::high_resolution_clock::time_point _end;
};