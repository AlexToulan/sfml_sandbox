#include "gtest/gtest.h"
#include "Utils/Timer.hpp"
#include <thread>

Timer timer;

class TimerTests : public testing::Test
{
protected:
  void SetUp() override
  {
    timer.start();
    timer.stop();
    _epsilonSeconds = 0.0001f;
  };

  void TearDown() override
  {

  };

  float _epsilonSeconds;
};

TEST_F(TimerTests, GetSeconds)
{
  timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  timer.stop();
  EXPECT_NEAR(timer.getSeconds(), 0.1f, _epsilonSeconds);
}

TEST_F(TimerTests, DeltaSeconds)
{
  timer.start();
  for (int i = 0; i < 10; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NEAR(timer.deltaSeconds(), 0.1f, _epsilonSeconds);
  }
}