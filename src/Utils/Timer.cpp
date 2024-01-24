#include "Utils/Timer.hpp"

Timer::Timer()
{
  start();
  stop();
}

Timer::~Timer()
{
  
}

void Timer::start()
{
  _start = hs_clock::now();
}

void Timer::stop()
{
  _end = hs_clock::now();
}

float Timer::getSeconds()
{
  return std::chrono::duration_cast<std::chrono::duration<double>>(_end - _start).count();
}

float Timer::pollSeconds()
{
  stop();
  float seconds = getSeconds();
  start();
  return seconds;
}