#include <thread>

#include "gtest/gtest.h"
#include "Utils/Logging.hpp"

#include "ProcessDataListener.hpp"
#include "ThreadEventListener.hpp"
#include "TestEvents.hpp"

class Events_ThreadEventListenerTest : public testing::Test
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

  ProcessDataListener requester;
  ThreadEventListener worker;
  std::vector<int> numbers;
};

TEST_F(Events_ThreadEventListenerTest, ThreadWorkerTest)
{
  TestEvents.subscribe(EventType::VECTOR_INT, requester.bind(&ProcessDataListener::receivedNumbers));
  TestEvents.subscribe(EventType::REQ_DOUBLE_INTS, worker.bind(&ThreadEventListener::receiveNumbers));
  requester.setOutNumbers(numbers);

  requester.send();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(numbers.size(), requester.getNumbers().size());
  for(int i = 0; i < numbers.size(); i++)
  {
    EXPECT_EQ(numbers[i] * 2, requester.getNumbers()[i]);
  }

  requester.clearInNumbers();
  requester.send();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(numbers.size(), requester.getNumbers().size());
  for(int i = 0; i < numbers.size(); i++)
  {
    EXPECT_EQ(numbers[i] * 2, requester.getNumbers()[i]);
  }
}
