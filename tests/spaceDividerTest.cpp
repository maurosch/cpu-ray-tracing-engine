#include <gtest/gtest.h>
#include "../src/hittables/space_divider.h"

// Demonstrate some basic assertions.
TEST(SpaceDivider, BasicAssertions) {
  SpaceDivider v(1, Box(point3(0,0,0), point3(3,3,3)));
  EXPECT_EQ(0, int(v.getCollisionItems(ray(point3(0,0,0), point3(1,1,1)), 0, 9999999999).size()));
}