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
  float deltaSeconds();

private:
  std::chrono::high_resolution_clock::time_point _start;
  std::chrono::high_resolution_clock::time_point _end;
};