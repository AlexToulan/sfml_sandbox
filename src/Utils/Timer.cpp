#include "Utils/Timer.hpp"

Timer::Timer()
{
  reset();
}

Timer::~Timer()
{
  
}

void Timer::reset()
{
  start();
  stop();
}

void Timer::start()
{
  _start = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
  _end = std::chrono::high_resolution_clock::now();
}

float Timer::getSeconds()
{
  return std::chrono::duration_cast<std::chrono::duration<double>>(_end - _start).count();
}

float Timer::pollSeconds()
{
  stop();
  return getSeconds();
}

float Timer::deltaSeconds()
{
  stop();
  float seconds = getSeconds();
  start();
  return seconds;
}

std::chrono::microseconds Timer::getMicroSecDuration()
{
  return std::chrono::duration_cast<std::chrono::microseconds>(_end - _start);
}
