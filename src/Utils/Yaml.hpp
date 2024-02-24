#pragma once

#include <yaml-cpp/yaml.h>
#include <SFML/Graphics/Color.hpp>

namespace YAML
{
  template<class T>
  Node encodeArray(const T* arr, size_t size)
  {
    Node node;
    for (size_t i = 0; i < size; i++)
    {
      node.push_back(arr[i]);
    }
    return node;
  }

  template<class T>
  bool decodeArray(const Node& node, T* arr, size_t size)
  {
    if(!node.IsSequence() || node.size() != size)
    {
      return false;
    }

    for (size_t i = 0; i < size; i++)
    {
      arr[i] = node[i].as<T>();
    }
    return true;
  }

  template<>
  struct convert<sf::Color> {
    static Node encode(const sf::Color& rhs) {
      Node node;
      node.push_back(rhs.r);
      node.push_back(rhs.g);
      node.push_back(rhs.b);
      node.push_back(rhs.a);
      return node;
    }

    static bool decode(const Node& node, sf::Color& rhs) {
      if(!node.IsSequence() || node.size() != 4) {
        return false;
      }

      rhs.r = node[0].as<sf::Uint8>();
      rhs.g = node[1].as<sf::Uint8>();
      rhs.b = node[2].as<sf::Uint8>();
      rhs.a = node[3].as<sf::Uint8>();
      return true;
    }
  };
}