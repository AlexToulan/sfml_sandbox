#include "ThreadWorker.hpp"

ThreadWorker::ThreadWorker()
{
  _isRunning = false;
}

ThreadWorker::~ThreadWorker()
{
}

bool ThreadWorker::start()
{
  if (_isRunning)
  {
    Log::warn("thread is already running");
    return false;
  }

  _isRunning = true;
  int errorCode = pthread_create(&_thread, nullptr, &_thread_bootstrap, (void*)this);
  if (errorCode != 0)
  {
    Log::error("failed to create pthread. error code: " + std::to_string(errorCode));
    _isRunning = false;
  }
  return _isRunning;
}

void ThreadWorker::stop()
{
  if (_isRunning)
  {
    _isRunning = false;
    pthread_join(_thread, nullptr);
  }
}

bool ThreadWorker::run()
{
  return _isRunning = false;
}