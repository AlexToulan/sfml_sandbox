#include "gtest/gtest.h"
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
  std::shared_ptr<int[]> testcpp20 = std::make_shared<int[]>(10);
  testcpp20[0] = 20;
  EXPECT_EQ(testcpp20[0], 20);
}
