#include "GameMode.hpp"

GameMode::GameMode()
{

}

GameMode::GameMode(const std::string& filePath)
{
  _name = filePath.substr(filePath.find_last_of("/") + 1, filePath.find_last_of("."));
}

GameMode::~GameMode()
{

}

std::string GameMode::getName() const
{
  return _name;
}