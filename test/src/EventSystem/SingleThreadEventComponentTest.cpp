#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventComponent.hpp"
#include "Utils/Logging.hpp"

#include "SingleThreadEventComponent.hpp"

class SingleThreadEventComponentTest : public testing::Test
{
protected:
  void SetUp() override
  {
    numbers_1 = { 5, 4, 3, 2, 1 };
    numbers_2 = { 1, 2, 3, 4, 5 };
  };

  void TearDown() override
  {
  };
  SingleThreadEventComponent a;
  SingleThreadEventComponent b;
  std::vector<int> numbers_1;
  std::vector<int> numbers_2;
};

TEST_F(SingleThreadEventComponentTest, Subscribe)
{
  a.subscribe(EventType::VECTOR_INT, &SingleThreadEventComponent::receivedNumbersEvent);
  b.subscribe(EventType::VECTOR_INT, &SingleThreadEventComponent::receivedNumbersEvent);

  Event numberEvent = Event(numbers_1);
  EventComponent::publish(EventType::VECTOR_INT, numberEvent);
  EXPECT_EQ(numbers_1, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  numberEvent.setData(numbers_2);
  EventComponent::publish(EventType::VECTOR_INT, numberEvent);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_2, b.getNumbers());
}

TEST_F(SingleThreadEventComponentTest, Unsubscribe)
{
  a.subscribe(EventType::VECTOR_INT, &SingleThreadEventComponent::receivedNumbersEvent);
  b.subscribe(EventType::VECTOR_INT, &SingleThreadEventComponent::receivedNumbersEvent);
  Event numberEvent = Event(numbers_1);

  EventComponent::publish(EventType::VECTOR_INT, numberEvent);
  EXPECT_EQ(numbers_1, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  numberEvent.setData(numbers_2);
  b.unsubscribe(EventType::VECTOR_INT);
  EventComponent::publish(EventType::VECTOR_INT, numberEvent);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  numberEvent.setData(numbers_1);
  a.unsubscribe(EventType::VECTOR_INT);
  EventComponent::publish(EventType::VECTOR_INT, numberEvent);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());
}