#include "GameMode.hpp"

GameMode::GameMode()
{
  _resourcesPath = "resources/Unknown/";
}

GameMode::GameMode(const std::string& filePath)
{
  size_t start = filePath.find_last_of("/") + 1;
  size_t end = filePath.find_last_of(".");
  _name = filePath.substr(start, end - start);

  _resourcesPath = "resources/" + _name + "/";
}

GameMode::~GameMode()
{

}

std::string GameMode::getName() const
{
  return _name;
}
