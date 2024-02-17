#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventComponent.hpp"
#include "Utils/Logging.hpp"

#include "SingleThreadEventComponent.hpp"
#include "ThreadEventComponent.hpp"

class ThreadEventComponentTest : public testing::Test
{
protected:
  void SetUp() override
  {
    numbers = {5, 4, 3, 2, 1};
    worker.start();
  };

  void TearDown() override
  {
    worker.stop();
  };

  SingleThreadEventComponent requester;
  ThreadEventComponent worker;
  std::vector<int> numbers;
};

TEST_F(ThreadEventComponentTest, ThreadWorkerTest)
{
  requester.subscribe(EventType::VECTOR_INT, &SingleThreadEventComponent::receivedNumbersEvent);
  worker.subscribe(EventType::REQ_DOUBLE_INTS, &ThreadEventComponent::receiveNumbers);
  requester.setOutNumbers(numbers);

  requester.send();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_EQ(numbers.size(), requester.getNumbers().size());
  for(int i = 0; i < numbers.size(); i++)
  {
    EXPECT_EQ(numbers[i] * 2, requester.getNumbers()[i]);
  }

  requester.clearInNumbers();
  requester.send();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_EQ(numbers.size(), requester.getNumbers().size());
  for(int i = 0; i < numbers.size(); i++)
  {
    EXPECT_EQ(numbers[i] * 2, requester.getNumbers()[i]);
  }
}
