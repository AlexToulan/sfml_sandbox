#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/Logging.hpp"

#include "SimpleListener.hpp"

class SimpleListenerTest : public testing::Test
{
protected:
  void SetUp() override
  {
    number = 1;
  };

  void TearDown() override
  {
    events.flushSubscribers();
  };
  SimpleListener a;
  int number;
  EventSystem<EventType> events;
};

TEST_F(SimpleListenerTest, Subscribe)
{
  events.subscribe(EventType::REQ_INC_INT, a.bind(&SimpleListener::receivedNumbersEvent));

  events.publish(EventType::REQ_INC_INT, Event(number));
  EXPECT_EQ(a.getNumber(), 1);
  events.publish(EventType::REQ_INC_INT, Event(number));
  EXPECT_EQ(a.getNumber(), 2);
}

TEST_F(SimpleListenerTest, MultipleSubscribe)
{
  events.subscribe(EventType::REQ_INC_INT, a.bind(&SimpleListener::receivedNumbersEvent));
  events.subscribe(EventType::REQ_INC_INT, a.bind(&SimpleListener::receivedNumbersEvent));

  events.publish(EventType::REQ_INC_INT, Event(number));
  EXPECT_EQ(a.getNumber(), 1);
  events.publish(EventType::REQ_INC_INT, Event(number));
  EXPECT_EQ(a.getNumber(), 2);
}

TEST_F(SimpleListenerTest, ScopedListener)
{
  {
    SimpleListener temp;
    events.subscribe(EventType::REQ_INC_INT, temp.bind(&SimpleListener::receivedNumbersEvent));
    events.publish(EventType::REQ_INC_INT, Event(number));
    EXPECT_EQ(temp.getNumber(), 1);
  }
  // publishing to a destroyed object
  EXPECT_FALSE(events.publish(EventType::REQ_INC_INT, Event(number)));

  {
    SimpleListener temp;
    events.subscribe(EventType::REQ_INC_INT, temp.bind(&SimpleListener::receivedNumbersEvent));
    events.publish(EventType::REQ_INC_INT, Event(number));
    EXPECT_EQ(temp.getNumber(), 1);
    // this time we cleanup our subscriptions
    events.unsubscribe(EventType::REQ_INC_INT, &temp);
  }
  EXPECT_TRUE(events.publish(EventType::REQ_INC_INT, Event(number)));
}
