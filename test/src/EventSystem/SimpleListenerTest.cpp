#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventSystem.hpp"
#include "Utils/Logging.hpp"

#include "SimpleListener.hpp"

class Events_SimpleListenerTest : public testing::Test
{
protected:
  void SetUp() override
  {
    number = 1;
  };

  void TearDown() override
  {
  };
  SimpleListener a;
  int number;
  EventSystem<EventType> events;
};

TEST_F(Events_SimpleListenerTest, Subscribe)
{
  events.subscribe(EventType::REQ_INC_INT, a.bind(&SimpleListener::receivedNumbersEvent));

  events.publish(EventType::REQ_INC_INT, number);
  EXPECT_EQ(a.getNumber(), number);
  events.publish(EventType::REQ_INC_INT, number);
  EXPECT_EQ(a.getNumber(), number * 2);
}

TEST_F(Events_SimpleListenerTest, MultipleSubscribe)
{
  events.subscribe(EventType::REQ_INC_INT, a.bind(&SimpleListener::receivedNumbersEvent));
  events.subscribe(EventType::REQ_INC_INT, a.bind(&SimpleListener::receivedNumbersEvent));

  events.publish(EventType::REQ_INC_INT, number);
  EXPECT_EQ(a.getNumber(), number);
  events.publish(EventType::REQ_INC_INT, number);
  EXPECT_EQ(a.getNumber(), number * 2);
}

TEST_F(Events_SimpleListenerTest, ScopedListener)
{
  {
    SimpleListener temp;
    events.subscribe(EventType::REQ_INC_INT, temp.bind(&SimpleListener::receivedNumbersEvent));
    events.publish(EventType::REQ_INC_INT, number);
    EXPECT_EQ(temp.getNumber(), number);
  }
  // we get an expected error here by publishing to a destroyed object
  EXPECT_FALSE(events.publish(EventType::REQ_INC_INT, number));
  events.pruneBindings();
  // bindings containing dangling references cleaned up
  EXPECT_TRUE(events.publish(EventType::REQ_INC_INT, number));
  
  {
    SimpleListener temp;
    events.subscribe(EventType::REQ_INC_INT, temp.bind(&SimpleListener::receivedNumbersEvent));
    events.publish(EventType::REQ_INC_INT, number);
    EXPECT_EQ(temp.getNumber(), number);
    // this time we cleanup our subscriptions
    events.unsubscribe(EventType::REQ_INC_INT, &temp);
  }
  EXPECT_TRUE(events.publish(EventType::REQ_INC_INT, number));
}
