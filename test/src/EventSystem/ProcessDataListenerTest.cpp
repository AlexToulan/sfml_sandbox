#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/Logging.hpp"

#include "ProcessDataListener.hpp"

class Events_ProcessDataListenerTest : public testing::Test
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
  ProcessDataListener a;
  ProcessDataListener b;
  std::vector<int> numbers_1;
  std::vector<int> numbers_2;
  EventSystem<EventType> events;
};

TEST_F(Events_ProcessDataListenerTest, MultipleSubscribe)
{
  events.subscribe(EventType::VECTOR_INT, a.bind(&ProcessDataListener::receivedNumbers));
  events.subscribe(EventType::VECTOR_INT, b.bind(&ProcessDataListener::receivedNumbers));

  events.publish(EventType::VECTOR_INT, numbers_1);
  EXPECT_EQ(numbers_1, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  events.publish(EventType::VECTOR_INT, numbers_2);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_2, b.getNumbers());
}

TEST_F(Events_ProcessDataListenerTest, Unsubscribe)
{
  events.subscribe(EventType::VECTOR_INT, a.bind(&ProcessDataListener::receivedNumbers));
  events.subscribe(EventType::VECTOR_INT, b.bind(&ProcessDataListener::receivedNumbers));

  events.publish(EventType::VECTOR_INT, numbers_1);
  EXPECT_EQ(numbers_1, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  events.unsubscribe(EventType::VECTOR_INT, &b);
  events.publish(EventType::VECTOR_INT, numbers_2);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  events.unsubscribe(EventType::VECTOR_INT, &a);
  events.publish(EventType::VECTOR_INT, numbers_1);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());
}
