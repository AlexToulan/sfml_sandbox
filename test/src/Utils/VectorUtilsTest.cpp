#include "gtest/gtest.h"
#include "Utils/VectorUtils.hpp"

#include <algorithm>
#include <execution>
#include <memory>


class VectorUtilsTest : public testing::Test
{
protected:
  void SetUp() override
  {

  };

  void TearDown() override
  {

  };
};

TEST_F(VectorUtilsTest, RayCast)
{
  vu::Hit hit;
  auto a0 = sf::Vector2f(0.0f, 0.0f);
  auto a1 = sf::Vector2f(1.0f, 1.0f);
  auto b0 = sf::Vector2f(1.0f, 0.0f);
  auto b1 = sf::Vector2f(0.0f, 1.0f);

  // crossing - yes
  vu::rayCast(hit, a0, a1, b0, b1);
  EXPECT_TRUE(hit.collision);
  EXPECT_EQ(hit.point, sf::Vector2f(0.5f, 0.5f));
  RecordProperty("ray cast: crossing determinate", hit.det);

  // parallel - no
  vu::rayCast(hit, a0, b0, b1, a1);
  EXPECT_FALSE(hit.collision);
  EXPECT_EQ(hit.point, sf::Vector2f());
  RecordProperty("ray cast: parallel determinate", hit.det);

  // tip to tip - yes
  vu::rayCast(hit, b0, a1, b1, a1);
  EXPECT_TRUE(hit.collision);
  EXPECT_EQ(hit.point, a1);
  RecordProperty("ray cast: tip to tip determinate", hit.det);

  // tail to tail - yes
  vu::rayCast(hit, a1, b0, a1, b1);
  EXPECT_TRUE(hit.collision);
  EXPECT_EQ(hit.point, a1);
  RecordProperty("ray cast: tail to tail determinate", hit.det);

  // overlap - no
  vu::rayCast(hit, a0, a1, a0, a1);
  EXPECT_FALSE(hit.collision);
  EXPECT_EQ(hit.point, sf::Vector2f());
  RecordProperty("ray cast: overlap determinate", hit.det);

  // project - yes
  vu::rayCast(hit, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.1f, 0.1f), sf::Vector2f(1.0f, 0.0f), sf::Vector2f(0.0f, 1.0f));
  EXPECT_TRUE(hit.collision);
  EXPECT_EQ(hit.point, sf::Vector2f(0.5f, 0.5f));
  RecordProperty("ray cast: project determinate", hit.det);
}
