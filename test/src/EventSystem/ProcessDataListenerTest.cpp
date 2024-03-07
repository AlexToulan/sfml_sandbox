#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/Logging.hpp"

#include "ProcessDataListener.hpp"

class ProcessDataListenerTest : public testing::Test
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

TEST_F(ProcessDataListenerTest, MultipleSubscribe)
{
  events.subscribe(EventType::VECTOR_INT, a.bind(&ProcessDataListener::receivedNumbersEvent));
  events.subscribe(EventType::VECTOR_INT, b.bind(&ProcessDataListener::receivedNumbersEvent));

  Event event = Event(numbers_1);
  events.publish(EventType::VECTOR_INT, event);
  EXPECT_EQ(numbers_1, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  event.setData(numbers_2);
  events.publish(EventType::VECTOR_INT, event);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_2, b.getNumbers());
}

TEST_F(ProcessDataListenerTest, Unsubscribe)
{
  events.subscribe(EventType::VECTOR_INT, a.bind(&ProcessDataListener::receivedNumbersEvent));
  events.subscribe(EventType::VECTOR_INT, b.bind(&ProcessDataListener::receivedNumbersEvent));
  Event event = Event(numbers_1);

  events.publish(EventType::VECTOR_INT, event);
  EXPECT_EQ(numbers_1, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  event.setData(numbers_2);
  events.unsubscribe(EventType::VECTOR_INT, &b);
  events.publish(EventType::VECTOR_INT, event);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());

  event.setData(numbers_1);
  events.unsubscribe(EventType::VECTOR_INT, &a);
  events.publish(EventType::VECTOR_INT, event);
  EXPECT_EQ(numbers_2, a.getNumbers());
  EXPECT_EQ(numbers_1, b.getNumbers());
}
