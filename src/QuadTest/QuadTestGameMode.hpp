#pragma once

#include <filesystem>
#include <fstream>
#include <random>
#include <array>

#include "Utils/GameMode/GameMode.hpp"

class QuadTestGameMode : public GameMode
{
public:
  QuadTestGameMode();
  virtual ~QuadTestGameMode();
  virtual void onStart();
  virtual void processEvents(sf::Event& event);
  virtual void update(float delta_seconds);
  virtual void render(sf::RenderWindow& window);
  virtual void onEnd();

private:
  int _width;
  int _height;
  float _cellSize;
  float _padding;
  int _vertsPerQuat;

  sf::Vertex* _verts;
  sf::VertexBuffer _cells;
  sf::Color* _swatch;

  std::mt19937 _gen;
  std::uniform_int_distribution<> _distrib;
};