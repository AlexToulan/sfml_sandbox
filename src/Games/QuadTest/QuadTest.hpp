#pragma once

#include <filesystem>
#include <fstream>
#include <random>
#include <array>

#include "GameMode/GameMode.hpp"

class QuadTest : public GameMode
{
public:
  QuadTest();
  virtual ~QuadTest();
  virtual void onStart();
  virtual sf::Vector2f onResize(int screenX, int screenY);
  virtual void processEvents(sf::Event& event);
  virtual void update(float ds);
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
  std::unique_ptr<sf::Color[]> _swatch;

  std::mt19937 _gen;
  std::uniform_int_distribution<> _distrib;
};