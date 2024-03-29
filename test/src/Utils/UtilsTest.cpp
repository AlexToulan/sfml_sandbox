#include "gtest/gtest.h"
#include "Utils/Str.hpp"

#include <algorithm>
#include <execution>
#include <memory>


class UtilsTests : public testing::Test
{
protected:
  void SetUp() override
  {

  };

  void TearDown() override
  {

  };
};

TEST_F(UtilsTests, StringFormat)
{
  std::string message;

  message = Str::format("bad", 0);
  EXPECT_EQ(message, "bad");

  message = Str::format("prefix {} suffix", "message");
  EXPECT_EQ(message, "prefix message suffix");

  // more args than replacement fields
  message = Str::format("1: {}, 2: {}", 1, 2, 3);
  EXPECT_EQ(message, "1: 1, 2: 2");

  // more replacement fields than args
  message = Str::format("1: {}, 2: {}, 3: {}, 4: {}", 1, 2);
  EXPECT_EQ(message, "1: 1, 2: 2, 3: {}, 4: {}");
}
