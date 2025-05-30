#include <gtest/gtest.h>
#include "Core/Math/Vec2.hpp"

using namespace lin;

TEST(Vec2Test, DefaultConstructor) {
    Vec2 v;
    EXPECT_EQ(v.x, 0.0);
    EXPECT_EQ(v.y, 0.0);
}

TEST(Vec2Test, ScalarConstructor) {
    Vec2 v(3.5);
    EXPECT_EQ(v.x, 3.5);
    EXPECT_EQ(v.y, 3.5);
}

TEST(Vec2Test, ComponentConstructor) {
    Vec2 v(1.0, 2.0);
    EXPECT_EQ(v.x, 1.0);
    EXPECT_EQ(v.y, 2.0);
}

TEST(Vec2Test, IndexOperatorRead) {
    Vec2 v(4.0, 5.0);
    EXPECT_EQ(v[0], 4.0);
    EXPECT_EQ(v[1], 5.0);
    EXPECT_THROW(v[2], std::out_of_range);
}

TEST(Vec2Test, IndexOperatorWrite) {
    Vec2 v;
    v[0] = 10.0;
    v[1] = 20.0;
    EXPECT_EQ(v.x, 10.0);
    EXPECT_EQ(v.y, 20.0);
    EXPECT_THROW(v[2] = 1.0, std::out_of_range);
}

TEST(Vec2Test, UnaryMinus) {
    Vec2 v(1.0, -2.0);
    Vec2 result = -v;
    EXPECT_EQ(result.x, -1.0);
    EXPECT_EQ(result.y, 2.0);
}

TEST(Vec2Test, Addition) {
    Vec2 a(1.0, 2.0);
    Vec2 b(3.0, 4.0);
    Vec2 c = a + b;
    EXPECT_EQ(c.x, 4.0);
    EXPECT_EQ(c.y, 6.0);
}

TEST(Vec2Test, AdditionAssignment) {
    Vec2 a(1.0, 2.0);
    Vec2 b(3.0, 4.0);
    a += b;
    EXPECT_EQ(a.x, 4.0);
    EXPECT_EQ(a.y, 6.0);
}

TEST(Vec2Test, Subtraction) {
    Vec2 a(5.0, 6.0);
    Vec2 b(2.0, 1.0);
    Vec2 c = a - b;
    EXPECT_EQ(c.x, 3.0);
    EXPECT_EQ(c.y, 5.0);
}

TEST(Vec2Test, SubtractionAssignment) {
    Vec2 a(5.0, 6.0);
    Vec2 b(2.0, 1.0);
    a -= b;
    EXPECT_EQ(a.x, 3.0);
    EXPECT_EQ(a.y, 5.0);
}

TEST(Vec2Test, ScalarMultiplication) {
    Vec2 v(2.0, -3.0);
    Vec2 result = v * 2.0;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, -6.0);
}

TEST(Vec2Test, ScalarMultiplicationAssignment) {
    Vec2 v(2.0, -3.0);
    v *= 2.0;
    EXPECT_EQ(v.x, 4.0);
    EXPECT_EQ(v.y, -6.0);
}

TEST(Vec2Test, ScalarDivision) {
    Vec2 v(4.0, -2.0);
    Vec2 result = v / 2.0;
    EXPECT_EQ(result.x, 2.0);
    EXPECT_EQ(result.y, -1.0);
}

TEST(Vec2Test, ScalarDivisionAssignment) {
    Vec2 v(4.0, -2.0);
    v /= 2.0;
    EXPECT_EQ(v.x, 2.0);
    EXPECT_EQ(v.y, -1.0);
}

TEST(Vec2Test, Equality) {
    Vec2 a(1.0, 2.0);
    Vec2 b(1.0, 2.0);
    Vec2 c(2.0, 3.0);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(Vec2Test, Inequality) {
    Vec2 a(1.0, 2.0);
    Vec2 b(2.0, 3.0);
    EXPECT_TRUE(a != b);
}

TEST(Vec2Test, SquaredLength) {
    Vec2 v(3.0, 4.0);
    EXPECT_EQ(v.squaredLength(), 25.0);
}

TEST(Vec2Test, Length) {
    Vec2 v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(Vec2Test, Normalized) {
    Vec2 v(3.0, 4.0);
    Vec2 n = v.normalized();
    EXPECT_NEAR(n.x, 0.6, 1e-10);
    EXPECT_NEAR(n.y, 0.8, 1e-10);
}

TEST(Vec2Test, NormalizeInPlace) {
    Vec2 v(3.0, 4.0);
    v.normalize();
    EXPECT_NEAR(v.x, 0.6, 1e-10);
    EXPECT_NEAR(v.y, 0.8, 1e-10);
}

TEST(Vec2Test, NormalizeZeroVector) {
    Vec2 v(0.0, 0.0);
    Vec2 n = v.normalized();
    EXPECT_EQ(n.x, 0.0);
    EXPECT_EQ(n.y, 0.0);
}

TEST(Vec2Test, IsApproxTrue) {
    Vec2 a(1.0, 2.0);
    Vec2 b(1.0000001, 2.0000001);
    EXPECT_TRUE(a.isApprox(b, 1e-4));
}

TEST(Vec2Test, IsApproxFalse) {
    Vec2 a(1.0, 2.0);
    Vec2 b(1.1, 2.1);
    EXPECT_FALSE(a.isApprox(b, 1e-4));
}
