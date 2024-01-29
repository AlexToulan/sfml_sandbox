#pragma once

#include <string>

#include "Utils/EventSystem/EventBase.hpp"

// class StringEvent : public EventBase
// {
// public:
//   StringEvent() : EventBase() {}
//   StringEvent(const StringEvent& other) : EventBase() { _message = other._message; }
//   std::string getMessage() const
//   {
//     return _message;
//   }
//   std::string _message;
// };

// class VectorIntEvent : public EventBase
// {
// public:
//   VectorIntEvent() : EventBase() {}
//   VectorIntEvent(const VectorIntEvent& other) : EventBase() { _numbers = other._numbers; }
//   const std::vector<int>& getNumbers() const
//   {
//     return _numbers;
//   }
//   std::vector<int> _numbers;
// };