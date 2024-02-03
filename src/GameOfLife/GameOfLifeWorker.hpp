#pragma once

#include "Utils/EventSystem/EventComponent.hpp"

class GameOfLifeWorker : public EventComponent
{
public:
  GameOfLifeWorker();
  virtual ~GameOfLifeWorker();
  bool start();
  void stop();
private:
  bool _isRunning;
  bool _calculatedNeighbors;

  bool run();

  pthread_t _thread;
  static void* _thread_bootstrap(void* s)
  {
    int exit_code = 0;
    GameOfLifeWorker* self = static_cast<GameOfLifeWorker*>(s);
    while(self->run())
    {

    }
    pthread_exit(&exit_code);

    return nullptr;
  }
};