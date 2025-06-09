#include <gtest/gtest.h>
#include "Core/Math/Vec3.hpp"

using namespace lin;

TEST(Vec3dTest, DefaultConstructor) {
    Vec3d v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Vec3dTest, SingleValueConstructor) {
    Vec3d v(3.0);
    EXPECT_DOUBLE_EQ(v.x, 3.0);
    EXPECT_DOUBLE_EQ(v.y, 3.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vec3dTest, ThreeValueConstructor) {
    Vec3d v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vec3dTest, ConversionConstructor) {
    Vec3f v(1.0f, 2.0f, 3.0f);
    Vec3d vd(v);
    EXPECT_DOUBLE_EQ(vd.x, 1.0);
    EXPECT_DOUBLE_EQ(vd.y, 2.0);
    EXPECT_DOUBLE_EQ(vd.z, 3.0);
}

TEST(Vec3dTest, SubscriptOperatorConst) {
    const Vec3d v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(Vec3dTest, SubscriptOperatorNonConst) {
    Vec3d v;
    v[0] = 4.0;
    v[1] = 5.0;
    v[2] = 6.0;
    EXPECT_DOUBLE_EQ(v.x, 4.0);
    EXPECT_DOUBLE_EQ(v.y, 5.0);
    EXPECT_DOUBLE_EQ(v.z, 6.0);
}

TEST(Vec3dTest, SubscriptOperatorOutOfRange) {
    Vec3d v;
    EXPECT_THROW(v[3], std::out_of_range);
    EXPECT_THROW(static_cast<const Vec3d&>(v)[3], std::out_of_range);
}

TEST(Vec3dTest, NegationOperator) {
    Vec3d v(1.0, -2.0, 3.0);
    Vec3d r = -v;
    EXPECT_DOUBLE_EQ(r.x, -1.0);
    EXPECT_DOUBLE_EQ(r.y, 2.0);
    EXPECT_DOUBLE_EQ(r.z, -3.0);
}

TEST(Vec3dTest, AdditionOperators) {
    Vec3d a(1.0, 2.0, 3.0);
    Vec3d b(4.0, 5.0, 6.0);
    Vec3d c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 5.0);
    EXPECT_DOUBLE_EQ(c.y, 7.0);
    EXPECT_DOUBLE_EQ(c.z, 9.0);
    a += b;
    EXPECT_DOUBLE_EQ(a.x, 5.0);
    EXPECT_DOUBLE_EQ(a.y, 7.0);
    EXPECT_DOUBLE_EQ(a.z, 9.0);
}

TEST(Vec3dTest, SubtractionOperators) {
    Vec3d a(5.0, 6.0, 7.0);
    Vec3d b(2.0, 3.0, 1.0);
    Vec3d c = a - b;
    EXPECT_DOUBLE_EQ(c.x, 3.0);
    EXPECT_DOUBLE_EQ(c.y, 3.0);
    EXPECT_DOUBLE_EQ(c.z, 6.0);
    a -= b;
    EXPECT_DOUBLE_EQ(a.x, 3.0);
    EXPECT_DOUBLE_EQ(a.y, 3.0);
    EXPECT_DOUBLE_EQ(a.z, 6.0);
}

TEST(Vec3dTest, ScalarMultiplication) {
    Vec3d v(1.0, -2.0, 3.0);
    Vec3d r = v * 2.0;
    EXPECT_DOUBLE_EQ(r.x, 2.0);
    EXPECT_DOUBLE_EQ(r.y, -4.0);
    EXPECT_DOUBLE_EQ(r.z, 6.0);
    v *= -1.0;
    EXPECT_DOUBLE_EQ(v.x, -1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, -3.0);
}

TEST(Vec3dTest, ScalarDivision) {
    Vec3d v(2.0, 4.0, 6.0);
    Vec3d r = v / 2.0;
    EXPECT_DOUBLE_EQ(r.x, 1.0);
    EXPECT_DOUBLE_EQ(r.y, 2.0);
    EXPECT_DOUBLE_EQ(r.z, 3.0);
    v /= 2.0;
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Vec3dTest, EqualityOperators) {
    Vec3d a(1.0, 2.0, 3.0);
    Vec3d b(1.0, 2.0, 3.0);
    Vec3d c(1.0, 0.0, 3.0);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

TEST(Vec3dTest, LengthAndSquaredLength) {
    Vec3d v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(v.squaredLength(), 25.0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(Vec3dTest, Normalize) {
    Vec3d v(0.0, 3.0, 4.0);
    v.normalize();
    EXPECT_NEAR(v.x, 0.0, 1e-12);
    EXPECT_NEAR(v.y, 0.6, 1e-12);
    EXPECT_NEAR(v.z, 0.8, 1e-12);
}

TEST(Vec3dTest, Normalized) {
    Vec3d v(0.0, 3.0, 4.0);
    Vec3d n = v.normalized();
    EXPECT_NEAR(n.x, 0.0, 1e-12);
    EXPECT_NEAR(n.y, 0.6, 1e-12);
    EXPECT_NEAR(n.z, 0.8, 1e-12);
}

TEST(Vec3dTest, CrossProduct) {
    Vec3d a(1.0, 0.0, 0.0);
    Vec3d b(0.0, 1.0, 0.0);
    Vec3d c = a.cross(b);
    EXPECT_DOUBLE_EQ(c.x, 0.0);
    EXPECT_DOUBLE_EQ(c.y, 0.0);
    EXPECT_DOUBLE_EQ(c.z, 1.0);
}

TEST(Vec3dTest, CwiseInverse) {
    Vec3d v(2.0, -4.0, 0.5);
    Vec3d inv = v.cwiseInverse();
    EXPECT_DOUBLE_EQ(inv.x, 0.5);
    EXPECT_DOUBLE_EQ(inv.y, -0.25);
    EXPECT_DOUBLE_EQ(inv.z, 2.0);
}

TEST(Vec3dTest, MinMaxValue) {
    Vec3d v(3.0, -7.0, 1.0);
    EXPECT_DOUBLE_EQ(v.minValue(), -7.0);
    EXPECT_DOUBLE_EQ(v.maxValue(), 3.0);
}

TEST(Vec3dTest, IsApprox) {
    Vec3d a(1.0, 2.0, 3.0);
    Vec3d b(1.00001, 2.00001, 2.99999);
    EXPECT_TRUE(a.isApprox(b, 1e-4));
    EXPECT_FALSE(a.isApprox(b, 1e-6));
}
