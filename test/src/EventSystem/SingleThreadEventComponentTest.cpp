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
    a.setup();
    b.setup();
    numbers = { 5, 4, 3, 2, 1 };
    a.setNumbers(numbers);
    b.setNumbers(numbers);
  };

  void TearDown() override
  {
  };
  SingleThreadEventComponent a;
  SingleThreadEventComponent b;
  std::vector<int> numbers;
};

TEST_F(SingleThreadEventComponentTest, SingleThreadEvent)
{
  // EXPECT_EQ(singleThread_a._message, "");
  // EventComponent::publish(EventType::STRING, stringEvent);
  // singleThread_a.run();
  // EXPECT_EQ(singleThread_a._message, stringEvent._message);
}

TEST_F(SingleThreadEventComponentTest, SingleThreadMultipleListenersEvent)
{
  // EXPECT_EQ(singleThread_a._message, "");
  // EXPECT_EQ(singleThread_b._message, "");
  // EventComponent::publish(EventType::STRING, stringEvent);
  // singleThread_a.run();
  // singleThread_b.run();
  // EXPECT_EQ(singleThread_a._message, stringEvent._message);
  // EXPECT_EQ(singleThread_b._message, stringEvent._message);
}