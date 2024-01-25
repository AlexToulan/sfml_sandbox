#include "gtest/gtest.h"
#include "Utils/Timer.hpp"
#include <thread>


class TimerTests : public testing::Test
{
protected:
  void SetUp() override
  {
    _timer.start();
    _timer.stop();
    _epsilonSeconds = 0.0001f;
  };

  void TearDown() override
  {

  };

  Timer _timer;
  float _epsilonSeconds;
};

TEST_F(TimerTests, GetSeconds)
{
  _timer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  _timer.stop();
  EXPECT_NEAR(_timer.getSeconds(), 0.1f, _epsilonSeconds);
}

TEST_F(TimerTests, DeltaSeconds)
{
  _timer.start();
  for (int i = 0; i < 10; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NEAR(_timer.deltaSeconds(), 0.1f, _epsilonSeconds);
  }
}