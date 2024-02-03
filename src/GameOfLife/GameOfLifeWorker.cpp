#include "EventTypes.hpp"
#include "GameOfLifeWorker.hpp"
#include "Utils/Logging.hpp"

GameOfLifeWorker::GameOfLifeWorker()
  : EventComponent()
{

}

GameOfLifeWorker::~GameOfLifeWorker()
{
  stop();
}

bool GameOfLifeWorker::start()
{
  _dataProcessed = true;
  _isRunning = true;

  if (pthread_create(&_thread, NULL, &_thread_bootstrap, (void*)this) != 0)
  {
    Log::error("failed to start thread");
    return false;
  }
  return true;
}

void GameOfLifeWorker::stop()
{
  if (_isRunning)
  {
    _isRunning = false;
    pthread_join(_thread, nullptr);
  }
}

bool GameOfLifeWorker::run()
{
  if (!_dataProcessed)
  {
    std::vector<int> doubled;
    std::unique_lock<decltype(_numbersMutex)> lock(_numbersMutex);
    for (size_t i = 0; i < _inNumbers.size(); i++)
    {
      doubled.push_back(_inNumbers[i] * 2);
    }
    EventComponent::publish(EventType::VECTOR_INT, Event(doubled));
    _dataProcessed = true;
  }
  return _isRunning;
}