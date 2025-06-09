#include <gtest/gtest.h>
#include "Core/Math/Vec4.hpp"

using namespace lin;

TEST(Vec4dTest, DefaultConstructor) {
  Vec4d v;
  EXPECT_EQ(v.x, 0.0);
  EXPECT_EQ(v.y, 0.0);
  EXPECT_EQ(v.z, 0.0);
  EXPECT_EQ(v.w, 0.0);
}

TEST(Vec4dTest, ScalarConstructor) {
  Vec4d v(5.0);
  EXPECT_EQ(v.x, 5.0);
  EXPECT_EQ(v.y, 5.0);
  EXPECT_EQ(v.z, 5.0);
  EXPECT_EQ(v.w, 5.0);
}

TEST(Vec4dTest, ComponentConstructor) {
  Vec4d v(1.0, 2.0, 3.0, 4.0);
  EXPECT_EQ(v.x, 1.0);
  EXPECT_EQ(v.y, 2.0);
  EXPECT_EQ(v.z, 3.0);
  EXPECT_EQ(v.w, 4.0);
}

TEST(Vec4dTest, ConversionConstructor) {
  Vec4f v(1.5f);
  Vec4d vd(v);
  EXPECT_EQ(vd.x, 1.5);
  EXPECT_EQ(vd.y, 1.5);
  EXPECT_EQ(vd.z, 1.5);
  EXPECT_EQ(vd.w, 1.5);
}

TEST(Vec4dTest, IndexOperatorConst) {
  const Vec4d v(1.0, 2.0, 3.0, 4.0);
  EXPECT_EQ(v[0], 1.0);
  EXPECT_EQ(v[1], 2.0);
  EXPECT_EQ(v[2], 3.0);
  EXPECT_EQ(v[3], 4.0);
}

TEST(Vec4dTest, IndexOperatorNonConst) {
  Vec4d v;
  v[0] = 1.0;
  v[1] = 2.0;
  v[2] = 3.0;
  v[3] = 4.0;
  EXPECT_EQ(v.x, 1.0);
  EXPECT_EQ(v.y, 2.0);
  EXPECT_EQ(v.z, 3.0);
  EXPECT_EQ(v.w, 4.0);
}

TEST(Vec4dTest, IndexOutOfRangeThrows) {
  Vec4d v;
  EXPECT_THROW(v[4], std::out_of_range);
  const Vec4d c;
  EXPECT_THROW(c[5], std::out_of_range);
}

TEST(Vec4dTest, UnaryMinus) {
  Vec4d v(1.0, -2.0, 3.0, -4.0);
  Vec4d r = -v;
  EXPECT_EQ(r, Vec4d(-1.0, 2.0, -3.0, 4.0));
}

TEST(Vec4dTest, Addition) {
  Vec4d a(1, 2, 3, 4);
  Vec4d b(5, 6, 7, 8);
  EXPECT_EQ(a + b, Vec4d(6, 8, 10, 12));
}

TEST(Vec4dTest, Subtraction) {
  Vec4d a(5, 6, 7, 8);
  Vec4d b(1, 2, 3, 4);
  EXPECT_EQ(a - b, Vec4d(4, 4, 4, 4));
}

TEST(Vec4dTest, ScalarMultiplication) {
  Vec4d a(1, -2, 3, -4);
  EXPECT_EQ(a * 2.0, Vec4d(2, -4, 6, -8));
}

TEST(Vec4dTest, ScalarDivision) {
  Vec4d a(2, 4, 6, 8);
  EXPECT_EQ(a / 2.0, Vec4d(1, 2, 3, 4));
}

TEST(Vec4dTest, CompoundAddition) {
  Vec4d a(1, 1, 1, 1);
  a += Vec4d(2, 3, 4, 5);
  EXPECT_EQ(a, Vec4d(3, 4, 5, 6));
}

TEST(Vec4dTest, CompoundSubtraction) {
  Vec4d a(5, 5, 5, 5);
  a -= Vec4d(1, 2, 3, 4);
  EXPECT_EQ(a, Vec4d(4, 3, 2, 1));
}

TEST(Vec4dTest, CompoundMultiplication) {
  Vec4d a(1, 2, 3, 4);
  a *= 2.0;
  EXPECT_EQ(a, Vec4d(2, 4, 6, 8));
}

TEST(Vec4dTest, CompoundDivision) {
  Vec4d a(2, 4, 6, 8);
  a /= 2.0;
  EXPECT_EQ(a, Vec4d(1, 2, 3, 4));
}

TEST(Vec4dTest, Equality) {
  EXPECT_TRUE(Vec4d(1, 2, 3, 4) == Vec4d(1, 2, 3, 4));
  EXPECT_FALSE(Vec4d(1, 2, 3, 4) == Vec4d(4, 3, 2, 1));
}

TEST(Vec4dTest, Inequality) {
  EXPECT_TRUE(Vec4d(1, 2, 3, 4) != Vec4d(4, 3, 2, 1));
  EXPECT_FALSE(Vec4d(1, 2, 3, 4) != Vec4d(1, 2, 3, 4));
}

TEST(Vec4dTest, SquaredLength) {
  Vec4d v(1, 2, 2, 1);
  EXPECT_DOUBLE_EQ(v.squaredLength(), 10.0);
}

TEST(Vec4dTest, Length) {
  Vec4d v(1, 2, 2, 1);
  EXPECT_DOUBLE_EQ(v.length(), std::sqrt(10.0));
}

TEST(Vec4dTest, NormalizeInPlace) {
  Vec4d v(2, 0, 0, 0);
  v.normalize();
  EXPECT_DOUBLE_EQ(v.x, 1.0);
  EXPECT_DOUBLE_EQ(v.y, 0.0);
  EXPECT_DOUBLE_EQ(v.z, 0.0);
  EXPECT_DOUBLE_EQ(v.w, 0.0);
}

TEST(Vec4dTest, Normalized) {
  Vec4d v(0, 3, 0, 0);
  Vec4d n = v.normalized();
  EXPECT_DOUBLE_EQ(n.x, 0.0);
  EXPECT_DOUBLE_EQ(n.y, 1.0);
  EXPECT_DOUBLE_EQ(n.z, 0.0);
  EXPECT_DOUBLE_EQ(n.w, 0.0);
}

TEST(Vec4dTest, IsApproxTrue) {
  Vec4d a(1.0001, 2.0001, 3.0001, 4.0001);
  Vec4d b(1.0002, 2.0002, 3.0002, 4.0002);
  EXPECT_TRUE(a.isApprox(b, 0.001));
}

TEST(Vec4dTest, IsApproxFalse) {
  Vec4d a(1.0, 2.0, 3.0, 4.0);
  Vec4d b(1.1, 2.1, 3.1, 4.1);
  EXPECT_FALSE(a.isApprox(b, 0.05));
}
