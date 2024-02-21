#include "gtest/gtest.h"

#include <algorithm>
#include <execution>
#include <memory>


class EnvironmentTests : public testing::Test
{
protected:
  void SetUp() override
  {

  };

  void TearDown() override
  {

  };
};

TEST_F(EnvironmentTests, CPP20)
{
  // can only use shared pointer arrays in c++20
  std::shared_ptr<int[]> testcpp20 = std::make_shared<int[]>(10);
  testcpp20[5] = 20;
  EXPECT_EQ(testcpp20[5], 20);
}

TEST_F(EnvironmentTests, ParallelForEach)
{
  std::vector<int> nums {1, 3, 5, 7, 9, 11, 13, 15};
  std::for_each(std::execution::par, nums.begin(), nums.end(),
    [](auto&& item)
      {
          item *= 2;
      });
  for (auto n : nums)
  {
    EXPECT_EQ(n % 2, 0);
  }
}
