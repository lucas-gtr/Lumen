#include <gtest/gtest.h>
#include "Core/Math/lin.hpp"

using namespace lin;

TEST(LinConversionTest, ToVec3) {
    Vec4 v4 = {1.0, 2.0, 3.0, 4.0};
    Vec3 v3 = toVec3(v4);
    EXPECT_DOUBLE_EQ(v3.x, 1.0);
    EXPECT_DOUBLE_EQ(v3.y, 2.0);
    EXPECT_DOUBLE_EQ(v3.z, 3.0);
}

TEST(LinConversionTest, ToVec4) {
    Vec3 v3 = {1.0, 2.0, 3.0};
    Vec4 v4 = toVec4(v3);
    EXPECT_DOUBLE_EQ(v4.x, 1.0);
    EXPECT_DOUBLE_EQ(v4.y, 2.0);
    EXPECT_DOUBLE_EQ(v4.z, 3.0);
    EXPECT_DOUBLE_EQ(v4.w, 1.0);
}

TEST(LinDotProductTest, Dot2D) {
    Vec2 a = {1.0, 2.0};
    Vec2 b = {3.0, 4.0};
    EXPECT_DOUBLE_EQ(dot(a, b), 11.0);
}

TEST(LinDotProductTest, Dot3D) {
    Vec3 a = {1.0, 2.0, 3.0};
    Vec3 b = {4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(dot(a, b), 32.0);
}

TEST(LinDotProductTest, Dot4D) {
    Vec4 a = {1.0, 2.0, 3.0, 4.0};
    Vec4 b = {5.0, 6.0, 7.0, 8.0};
    EXPECT_DOUBLE_EQ(dot(a, b), 70.0);
}

TEST(LinComponentWiseTest, MinMaxProduct2D) {
    Vec2 a = {1.0, 5.0};
    Vec2 b = {3.0, 2.0};
    EXPECT_EQ(cwiseMin(a, b), Vec2(1.0, 2.0));
    EXPECT_EQ(cwiseMax(a, b), Vec2(3.0, 5.0));
    EXPECT_EQ(cwiseProduct(a, b), Vec2(3.0, 10.0));
}

TEST(LinComponentWiseTest, MinMaxProduct3D) {
    Vec3 a = {1.0, 5.0, -2.0};
    Vec3 b = {3.0, 2.0, 4.0};
    EXPECT_EQ(cwiseMin(a, b), Vec3(1.0, 2.0, -2.0));
    EXPECT_EQ(cwiseMax(a, b), Vec3(3.0, 5.0, 4.0));
    EXPECT_EQ(cwiseProduct(a, b), Vec3(3.0, 10.0, -8.0));
}

TEST(LinComponentWiseTest, MinMaxProduct4D) {
    Vec4 a = {1.0, 5.0, -2.0, 3.0};
    Vec4 b = {3.0, 2.0, 4.0, 7.0};
    EXPECT_EQ(cwiseMin(a, b), Vec4(1.0, 2.0, -2.0, 3.0));
    EXPECT_EQ(cwiseMax(a, b), Vec4(3.0, 5.0, 4.0, 7.0));
    EXPECT_EQ(cwiseProduct(a, b), Vec4(3.0, 10.0, -8.0, 21.0));
}

TEST(LinScalarTest, ScalarMultiplication) {
    Vec2 v2 = {2.0, -1.0};
    Vec3 v3 = {1.0, 0.0, -1.0};
    Vec4 v4 = {1.0, 2.0, 3.0, 4.0};
    EXPECT_EQ(2.0 * v2, Vec2(4.0, -2.0));
    EXPECT_EQ(3.0 * v3, Vec3(3.0, 0.0, -3.0));
    EXPECT_EQ(0.5 * v4, Vec4(0.5, 1.0, 1.5, 2.0));
}

TEST(LinMatrixTest, Mat3Vec3Multiplication) {
    Mat3 m({{1, 2, 3}, {0, 1, 4}, {5, 6, 0}});
    Vec3 v = {1, 2, 3};
    Vec3 result = m * v;
    EXPECT_EQ(result, Vec3(14.0, 14.0, 17.0));
}

TEST(LinMatrixTest, Mat4Vec4Multiplication) {
    Mat4 m({{1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}});
    Vec4 v = {1, 2, 3, 4};
    Vec4 result = m * v;
    EXPECT_EQ(result, Vec4(30, 70, 110, 150));
}

TEST(LinRotationTest, RotationMatrixOrthogonality) {
    Mat3 rot = getRotationMatrix(0.1, 0.2, 0.3);
    Mat3 transposed = rot.transposed();
    Mat3 identity = rot * transposed;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_NEAR(identity.m[i][j], i == j ? 1.0 : 0.0, 1e-9);
}
