#pragma once

#include <pthread.h>

#include "Utils/Logging.hpp"

class ThreadWorker
{
  ThreadWorker();
  ThreadWorker (const ThreadWorker& other) = delete;
  virtual ~ThreadWorker();

protected:
  bool start();
  void stop();
  virtual bool run() {};

  pthread_t _thread;
  static void* _thread_bootstrap(void* s)
  {
    int exit_code = 0;
    ThreadWorker* self = static_cast<ThreadWorker*>(s);
    while(self->run())
    {

    }
    pthread_exit(&exit_code);
    return nullptr;
  }

private:
  bool _isRunning;
};
