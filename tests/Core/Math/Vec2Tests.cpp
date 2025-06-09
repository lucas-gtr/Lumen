#include <gtest/gtest.h>
#include "Core/Math/Vec2.hpp"

using namespace lin;

TEST(Vec2dTest, DefaultConstructor) {
    Vec2d v;
    EXPECT_EQ(v.x, 0.0);
    EXPECT_EQ(v.y, 0.0);
}

TEST(Vec2dTest, ScalarConstructor) {
    Vec2d v(3.5);
    EXPECT_EQ(v.x, 3.5);
    EXPECT_EQ(v.y, 3.5);
}

TEST(Vec2dTest, ComponentConstructor) {
    Vec2d v(1.0, 2.0);
    EXPECT_EQ(v.x, 1.0);
    EXPECT_EQ(v.y, 2.0);
}

TEST(Vec2dTest, ConversionConstructor) {
    Vec2f v(1.0f, 2.0f);
    Vec2d vd(v);
    EXPECT_EQ(vd.x, 1.0);
    EXPECT_EQ(vd.y, 2.0);
}

TEST(Vec2dTest, IndexOperatorRead) {
    Vec2d v(4.0, 5.0);
    EXPECT_EQ(v[0], 4.0);
    EXPECT_EQ(v[1], 5.0);
    EXPECT_THROW(v[2], std::out_of_range);
}

TEST(Vec2dTest, IndexOperatorWrite) {
    Vec2d v;
    v[0] = 10.0;
    v[1] = 20.0;
    EXPECT_EQ(v.x, 10.0);
    EXPECT_EQ(v.y, 20.0);
    EXPECT_THROW(v[2] = 1.0, std::out_of_range);
}

TEST(Vec2dTest, UnaryMinus) {
    Vec2d v(1.0, -2.0);
    Vec2d result = -v;
    EXPECT_EQ(result.x, -1.0);
    EXPECT_EQ(result.y, 2.0);
}

TEST(Vec2dTest, Addition) {
    Vec2d a(1.0, 2.0);
    Vec2d b(3.0, 4.0);
    Vec2d c = a + b;
    EXPECT_EQ(c.x, 4.0);
    EXPECT_EQ(c.y, 6.0);
}

TEST(Vec2dTest, AdditionAssignment) {
    Vec2d a(1.0, 2.0);
    Vec2d b(3.0, 4.0);
    a += b;
    EXPECT_EQ(a.x, 4.0);
    EXPECT_EQ(a.y, 6.0);
}

TEST(Vec2dTest, Subtraction) {
    Vec2d a(5.0, 6.0);
    Vec2d b(2.0, 1.0);
    Vec2d c = a - b;
    EXPECT_EQ(c.x, 3.0);
    EXPECT_EQ(c.y, 5.0);
}

TEST(Vec2dTest, SubtractionAssignment) {
    Vec2d a(5.0, 6.0);
    Vec2d b(2.0, 1.0);
    a -= b;
    EXPECT_EQ(a.x, 3.0);
    EXPECT_EQ(a.y, 5.0);
}

TEST(Vec2dTest, ScalarMultiplication) {
    Vec2d v(2.0, -3.0);
    Vec2d result = v * 2.0;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, -6.0);
}

TEST(Vec2dTest, ScalarMultiplicationAssignment) {
    Vec2d v(2.0, -3.0);
    v *= 2.0;
    EXPECT_EQ(v.x, 4.0);
    EXPECT_EQ(v.y, -6.0);
}

TEST(Vec2dTest, ScalarDivision) {
    Vec2d v(4.0, -2.0);
    Vec2d result = v / 2.0;
    EXPECT_EQ(result.x, 2.0);
    EXPECT_EQ(result.y, -1.0);
}

TEST(Vec2dTest, ScalarDivisionAssignment) {
    Vec2d v(4.0, -2.0);
    v /= 2.0;
    EXPECT_EQ(v.x, 2.0);
    EXPECT_EQ(v.y, -1.0);
}

TEST(Vec2dTest, Equality) {
    Vec2d a(1.0, 2.0);
    Vec2d b(1.0, 2.0);
    Vec2d c(2.0, 3.0);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(Vec2dTest, Inequality) {
    Vec2d a(1.0, 2.0);
    Vec2d b(2.0, 3.0);
    EXPECT_TRUE(a != b);
}

TEST(Vec2dTest, SquaredLength) {
    Vec2d v(3.0, 4.0);
    EXPECT_EQ(v.squaredLength(), 25.0);
}

TEST(Vec2dTest, Length) {
    Vec2d v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(Vec2dTest, Normalized) {
    Vec2d v(3.0, 4.0);
    Vec2d n = v.normalized();
    EXPECT_NEAR(n.x, 0.6, 1e-10);
    EXPECT_NEAR(n.y, 0.8, 1e-10);
}

TEST(Vec2dTest, NormalizeInPlace) {
    Vec2d v(3.0, 4.0);
    v.normalize();
    EXPECT_NEAR(v.x, 0.6, 1e-10);
    EXPECT_NEAR(v.y, 0.8, 1e-10);
}

TEST(Vec2dTest, NormalizeZeroVector) {
    Vec2d v(0.0, 0.0);
    Vec2d n = v.normalized();
    EXPECT_EQ(n.x, 0.0);
    EXPECT_EQ(n.y, 0.0);
}

TEST(Vec2dTest, IsApproxTrue) {
    Vec2d a(1.0, 2.0);
    Vec2d b(1.0000001, 2.0000001);
    EXPECT_TRUE(a.isApprox(b, 1e-4));
}

TEST(Vec2dTest, IsApproxFalse) {
    Vec2d a(1.0, 2.0);
    Vec2d b(1.1, 2.1);
    EXPECT_FALSE(a.isApprox(b, 1e-4));
}
