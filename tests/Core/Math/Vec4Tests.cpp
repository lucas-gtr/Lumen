#include <gtest/gtest.h>
#include "Core/Math/Vec4.hpp"

using namespace lin;

TEST(Vec4Test, DefaultConstructor) {
  Vec4 v;
  EXPECT_EQ(v.x, 0.0);
  EXPECT_EQ(v.y, 0.0);
  EXPECT_EQ(v.z, 0.0);
  EXPECT_EQ(v.w, 0.0);
}

TEST(Vec4Test, ScalarConstructor) {
  Vec4 v(5.0);
  EXPECT_EQ(v.x, 5.0);
  EXPECT_EQ(v.y, 5.0);
  EXPECT_EQ(v.z, 5.0);
  EXPECT_EQ(v.w, 5.0);
}

TEST(Vec4Test, ComponentConstructor) {
  Vec4 v(1.0, 2.0, 3.0, 4.0);
  EXPECT_EQ(v.x, 1.0);
  EXPECT_EQ(v.y, 2.0);
  EXPECT_EQ(v.z, 3.0);
  EXPECT_EQ(v.w, 4.0);
}

TEST(Vec4Test, IndexOperatorConst) {
  const Vec4 v(1.0, 2.0, 3.0, 4.0);
  EXPECT_EQ(v[0], 1.0);
  EXPECT_EQ(v[1], 2.0);
  EXPECT_EQ(v[2], 3.0);
  EXPECT_EQ(v[3], 4.0);
}

TEST(Vec4Test, IndexOperatorNonConst) {
  Vec4 v;
  v[0] = 1.0;
  v[1] = 2.0;
  v[2] = 3.0;
  v[3] = 4.0;
  EXPECT_EQ(v.x, 1.0);
  EXPECT_EQ(v.y, 2.0);
  EXPECT_EQ(v.z, 3.0);
  EXPECT_EQ(v.w, 4.0);
}

TEST(Vec4Test, IndexOutOfRangeThrows) {
  Vec4 v;
  EXPECT_THROW(v[4], std::out_of_range);
  const Vec4 c;
  EXPECT_THROW(c[5], std::out_of_range);
}

TEST(Vec4Test, UnaryMinus) {
  Vec4 v(1.0, -2.0, 3.0, -4.0);
  Vec4 r = -v;
  EXPECT_EQ(r, Vec4(-1.0, 2.0, -3.0, 4.0));
}

TEST(Vec4Test, Addition) {
  Vec4 a(1, 2, 3, 4);
  Vec4 b(5, 6, 7, 8);
  EXPECT_EQ(a + b, Vec4(6, 8, 10, 12));
}

TEST(Vec4Test, Subtraction) {
  Vec4 a(5, 6, 7, 8);
  Vec4 b(1, 2, 3, 4);
  EXPECT_EQ(a - b, Vec4(4, 4, 4, 4));
}

TEST(Vec4Test, ScalarMultiplication) {
  Vec4 a(1, -2, 3, -4);
  EXPECT_EQ(a * 2.0, Vec4(2, -4, 6, -8));
}

TEST(Vec4Test, ScalarDivision) {
  Vec4 a(2, 4, 6, 8);
  EXPECT_EQ(a / 2.0, Vec4(1, 2, 3, 4));
}

TEST(Vec4Test, CompoundAddition) {
  Vec4 a(1, 1, 1, 1);
  a += Vec4(2, 3, 4, 5);
  EXPECT_EQ(a, Vec4(3, 4, 5, 6));
}

TEST(Vec4Test, CompoundSubtraction) {
  Vec4 a(5, 5, 5, 5);
  a -= Vec4(1, 2, 3, 4);
  EXPECT_EQ(a, Vec4(4, 3, 2, 1));
}

TEST(Vec4Test, CompoundMultiplication) {
  Vec4 a(1, 2, 3, 4);
  a *= 2.0;
  EXPECT_EQ(a, Vec4(2, 4, 6, 8));
}

TEST(Vec4Test, CompoundDivision) {
  Vec4 a(2, 4, 6, 8);
  a /= 2.0;
  EXPECT_EQ(a, Vec4(1, 2, 3, 4));
}

TEST(Vec4Test, Equality) {
  EXPECT_TRUE(Vec4(1, 2, 3, 4) == Vec4(1, 2, 3, 4));
  EXPECT_FALSE(Vec4(1, 2, 3, 4) == Vec4(4, 3, 2, 1));
}

TEST(Vec4Test, Inequality) {
  EXPECT_TRUE(Vec4(1, 2, 3, 4) != Vec4(4, 3, 2, 1));
  EXPECT_FALSE(Vec4(1, 2, 3, 4) != Vec4(1, 2, 3, 4));
}

TEST(Vec4Test, SquaredLength) {
  Vec4 v(1, 2, 2, 1);
  EXPECT_DOUBLE_EQ(v.squaredLength(), 10.0);
}

TEST(Vec4Test, Length) {
  Vec4 v(1, 2, 2, 1);
  EXPECT_DOUBLE_EQ(v.length(), std::sqrt(10.0));
}

TEST(Vec4Test, NormalizeInPlace) {
  Vec4 v(2, 0, 0, 0);
  v.normalize();
  EXPECT_DOUBLE_EQ(v.x, 1.0);
  EXPECT_DOUBLE_EQ(v.y, 0.0);
  EXPECT_DOUBLE_EQ(v.z, 0.0);
  EXPECT_DOUBLE_EQ(v.w, 0.0);
}

TEST(Vec4Test, Normalized) {
  Vec4 v(0, 3, 0, 0);
  Vec4 n = v.normalized();
  EXPECT_DOUBLE_EQ(n.x, 0.0);
  EXPECT_DOUBLE_EQ(n.y, 1.0);
  EXPECT_DOUBLE_EQ(n.z, 0.0);
  EXPECT_DOUBLE_EQ(n.w, 0.0);
}

TEST(Vec4Test, IsApproxTrue) {
  Vec4 a(1.0001, 2.0001, 3.0001, 4.0001);
  Vec4 b(1.0002, 2.0002, 3.0002, 4.0002);
  EXPECT_TRUE(a.isApprox(b, 0.001));
}

TEST(Vec4Test, IsApproxFalse) {
  Vec4 a(1.0, 2.0, 3.0, 4.0);
  Vec4 b(1.1, 2.1, 3.1, 4.1);
  EXPECT_FALSE(a.isApprox(b, 0.05));
}
