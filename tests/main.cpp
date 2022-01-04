#include <gtest/gtest.h>
#include "../src/utils/matrix.h"
#include "../src/utils/vec3.h"

TEST(MatrixMult, multiplyByIdentityGivesTheSame) {
  vector<vector<double>> m = {{1,0,0},{0,1,0},{0,0,1}};
  EXPECT_TRUE(matrix3(m) * vec3(1,1,1) == vec3(1,1,1));
}
TEST(MatrixMult, multiplyByIdentityGivesTheSame2) {
  vector<vector<double>> m = {{1,0,0},{0,1,0},{0,0,1}};
  EXPECT_TRUE(matrix3(m) * vec3(2,3,4) == vec3(2,3,4));
}
TEST(MatrixMult, rot90X) {
  const auto ang = 90 * pi / 180;
  matrix3 rotationXMatrix = matrix3({
        {1, 0, 0},
        {0, cosf(ang), sinf(ang)},
        {0, -sinf(ang), cosf(ang)},
  });;
  EXPECT_TRUE((rotationXMatrix * vec3(1,0,0) - vec3(1,0,0)).near_zero());
}
TEST(MatrixMult, rot90Y) {
  const auto angR = 90 * pi / 180;
  matrix3 rotationYMatrix = matrix3({
        { cos(angR), 0, -sin(angR) },
        { 0, 1, 0 },
        { sin(angR), 0, cos(angR) }
    });
  EXPECT_TRUE((rotationYMatrix * vec3(1,0,0) - vec3(0,0,1)).near_zero());
}