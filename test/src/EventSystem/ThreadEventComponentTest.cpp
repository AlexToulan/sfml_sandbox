#include <pthread.h>

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
    requester.setup();
    numbers = {5, 4, 3, 2, 1};
    requester.setNumbers(numbers);

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
  requester.send();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(numbers.size(), requester.getNumbers().size());
  for(int i = 0; i < numbers.size(); i++)
  {
    EXPECT_EQ(numbers[i] * 2, requester.getNumbers()[i]);
  }
}