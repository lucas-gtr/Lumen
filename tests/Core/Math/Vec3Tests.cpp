#include <gtest/gtest.h>
#include "Core/Math/Vec3.hpp"

using namespace lin;

TEST(Vec3Test, DefaultConstructor) {
    Vec3 v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vec3Test, SingleValueConstructor) {
    Vec3 v(3.0);
    EXPECT_DOUBLE_EQ(v.x, 3.0);
    EXPECT_DOUBLE_EQ(v.y, 3.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vec3Test, ThreeValueConstructor) {
    Vec3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vec3Test, SubscriptOperatorConst) {
    const Vec3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(Vec3Test, SubscriptOperatorNonConst) {
    Vec3 v;
    v[0] = 4.0;
    v[1] = 5.0;
    v[2] = 6.0;
    EXPECT_DOUBLE_EQ(v.x, 4.0);
    EXPECT_DOUBLE_EQ(v.y, 5.0);
    EXPECT_DOUBLE_EQ(v.z, 6.0);
}

TEST(Vec3Test, SubscriptOperatorOutOfRange) {
    Vec3 v;
    EXPECT_THROW(v[3], std::out_of_range);
    EXPECT_THROW(static_cast<const Vec3&>(v)[3], std::out_of_range);
}

TEST(Vec3Test, NegationOperator) {
    Vec3 v(1.0, -2.0, 3.0);
    Vec3 r = -v;
    EXPECT_DOUBLE_EQ(r.x, -1.0);
    EXPECT_DOUBLE_EQ(r.y, 2.0);
    EXPECT_DOUBLE_EQ(r.z, -3.0);
}

TEST(Vec3Test, AdditionOperators) {
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(4.0, 5.0, 6.0);
    Vec3 c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 5.0);
    EXPECT_DOUBLE_EQ(c.y, 7.0);
    EXPECT_DOUBLE_EQ(c.z, 9.0);
    a += b;
    EXPECT_DOUBLE_EQ(a.x, 5.0);
    EXPECT_DOUBLE_EQ(a.y, 7.0);
    EXPECT_DOUBLE_EQ(a.z, 9.0);
}

TEST(Vec3Test, SubtractionOperators) {
    Vec3 a(5.0, 6.0, 7.0);
    Vec3 b(2.0, 3.0, 1.0);
    Vec3 c = a - b;
    EXPECT_DOUBLE_EQ(c.x, 3.0);
    EXPECT_DOUBLE_EQ(c.y, 3.0);
    EXPECT_DOUBLE_EQ(c.z, 6.0);
    a -= b;
    EXPECT_DOUBLE_EQ(a.x, 3.0);
    EXPECT_DOUBLE_EQ(a.y, 3.0);
    EXPECT_DOUBLE_EQ(a.z, 6.0);
}

TEST(Vec3Test, ScalarMultiplication) {
    Vec3 v(1.0, -2.0, 3.0);
    Vec3 r = v * 2.0;
    EXPECT_DOUBLE_EQ(r.x, 2.0);
    EXPECT_DOUBLE_EQ(r.y, -4.0);
    EXPECT_DOUBLE_EQ(r.z, 6.0);
    v *= -1.0;
    EXPECT_DOUBLE_EQ(v.x, -1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, -3.0);
}

TEST(Vec3Test, ScalarDivision) {
    Vec3 v(2.0, 4.0, 6.0);
    Vec3 r = v / 2.0;
    EXPECT_DOUBLE_EQ(r.x, 1.0);
    EXPECT_DOUBLE_EQ(r.y, 2.0);
    EXPECT_DOUBLE_EQ(r.z, 3.0);
    v /= 2.0;
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vec3Test, EqualityOperators) {
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(1.0, 2.0, 3.0);
    Vec3 c(1.0, 0.0, 3.0);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

TEST(Vec3Test, LengthAndSquaredLength) {
    Vec3 v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(v.squaredLength(), 25.0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(Vec3Test, Normalize) {
    Vec3 v(0.0, 3.0, 4.0);
    v.normalize();
    EXPECT_NEAR(v.x, 0.0, 1e-12);
    EXPECT_NEAR(v.y, 0.6, 1e-12);
    EXPECT_NEAR(v.z, 0.8, 1e-12);
}

TEST(Vec3Test, Normalized) {
    Vec3 v(0.0, 3.0, 4.0);
    Vec3 n = v.normalized();
    EXPECT_NEAR(n.x, 0.0, 1e-12);
    EXPECT_NEAR(n.y, 0.6, 1e-12);
    EXPECT_NEAR(n.z, 0.8, 1e-12);
}

TEST(Vec3Test, CrossProduct) {
    Vec3 a(1.0, 0.0, 0.0);
    Vec3 b(0.0, 1.0, 0.0);
    Vec3 c = a.cross(b);
    EXPECT_DOUBLE_EQ(c.x, 0.0);
    EXPECT_DOUBLE_EQ(c.y, 0.0);
    EXPECT_DOUBLE_EQ(c.z, 1.0);
}

TEST(Vec3Test, CwiseInverse) {
    Vec3 v(2.0, -4.0, 0.5);
    Vec3 inv = v.cwiseInverse();
    EXPECT_DOUBLE_EQ(inv.x, 0.5);
    EXPECT_DOUBLE_EQ(inv.y, -0.25);
    EXPECT_DOUBLE_EQ(inv.z, 2.0);
}

TEST(Vec3Test, MinMaxValue) {
    Vec3 v(3.0, -7.0, 1.0);
    EXPECT_DOUBLE_EQ(v.minValue(), -7.0);
    EXPECT_DOUBLE_EQ(v.maxValue(), 3.0);
}

TEST(Vec3Test, IsApprox) {
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(1.00001, 2.00001, 2.99999);
    EXPECT_TRUE(a.isApprox(b, 1e-4));
    EXPECT_FALSE(a.isApprox(b, 1e-6));
}
