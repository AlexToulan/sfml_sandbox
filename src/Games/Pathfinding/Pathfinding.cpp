#include "Pathfinding.hpp"
#include "Utils/Logging.hpp"
#include "GameMode/ConsoleEvents.hpp"
#include "GameMode/DebugEvents.hpp"

Pathfinding::Pathfinding(sf::Vector2u originalScreenSize)
 : GameMode(originalScreenSize)
{
}

Pathfinding::~Pathfinding()
{

}

void Pathfinding::onStart()
{
  _graph = std::make_shared<PathGraph>();
  _graph->setup(_originalScreenSize, 25, 1);
  // _rayPath.addObstacle(std::vector<sf::Vector2f> { sf::Vector2f(500, -1000), sf::Vector2f(500, 900), sf::Vector2f(600, 200), sf::Vector2f(600, 2000) });
  _rayPath.loadMap(_resourcesPath + "/four_boxes.yaml");
}

void Pathfinding::processEvents(sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Right)
    {
      auto mousePos = getMousePos(event.mouseButton.x, event.mouseButton.y);
      auto a = _testAgent.getPosition();
      auto path = _rayPath.calculateCompletePath(a, mousePos);
      for (size_t i = 1; i < path.size(); i++)
        Events::Debug->publish(EDebugEvents::LINE, path[i - 1], path[i], sf::Color::White, 1.0f);
      // Events::Console->publish<std::string>("notify", Str::format("dot {}", vu::dot(p, vu::normalized(mousePos - a))));
      if (path.size() > 0)
        _testAgent.setPosition(path.back());
    }
  }
  if (event.type == sf::Event::MouseMoved)
  {
    _graph->hover(getMousePos(event.mouseMove.x, event.mouseMove.y));
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9))
  {
    _graph->save("grid.png");
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10))
  {
    _graph->load("grid.png");
  }
}

void Pathfinding::update(float ds)
{
}

void Pathfinding::render(sf::RenderWindow& window)
{
  window.draw(*_graph);
  window.draw(_testAgent);
}

void Pathfinding::onEnd()
{
}
