#include <gtest/gtest.h>
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Mat3.hpp"

using namespace lin;

TEST(Mat4Test, DefaultConstructor) {
  Mat4 mat;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_DOUBLE_EQ(mat(i, j), i == j ? 1.0 : 0.0);
}

TEST(Mat4Test, ScalarConstructor) {
  Mat4 mat(2.5);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_DOUBLE_EQ(mat(i, j), 2.5);
}

TEST(Mat4Test, InitializerListConstructor) {
  Mat4 mat({
    {1.0, 2.0, 3.0, 4.0},
    {5.0, 6.0, 7.0, 8.0},
    {9.0, 10.0, 11.0, 12.0},
    {13.0, 14.0, 15.0, 16.0}
  });

  int value = 1;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j, ++value)
      EXPECT_DOUBLE_EQ(mat(i, j), static_cast<double>(value));
}

TEST(Mat4Test, Mat3Constructor) {
  Mat3 mat3 = {{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat4 mat4(mat3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_DOUBLE_EQ(mat4(i, j), mat3(i, j));

  for (int i = 0; i < 3; ++i)
    EXPECT_DOUBLE_EQ(mat4(i, 3), 0.0);

  for (int j = 0; j < 3; ++j)
    EXPECT_DOUBLE_EQ(mat4(3, j), 0.0);

  EXPECT_DOUBLE_EQ(mat4(3, 3), 1.0);
}

TEST(Mat4Test, ElementAccessOperators) {
  Mat4 mat;
  mat(2, 3) = 42.0;
  EXPECT_DOUBLE_EQ(mat(2, 3), 42.0);

  mat[7] = 12.0;
  EXPECT_DOUBLE_EQ(mat[7], 12.0);
  EXPECT_DOUBLE_EQ(mat(1, 3), 12.0);
}

TEST(Mat4Test, EqualityOperators) {
  Mat4 a(1.0);
  Mat4 b(1.0);
  Mat4 c(2.0);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
  EXPECT_FALSE(a != b);
}

TEST(Mat4Test, Transpose) {
  Mat4 mat({
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  });

  Mat4 expected({
    {1, 5, 9, 13},
    {2, 6, 10, 14},
    {3, 7, 11, 15},
    {4, 8, 12, 16}
  });

  EXPECT_EQ(mat.transposed(), expected);
}

TEST(Mat4Test, Multiplication) {
  Mat4 identity;
  Mat4 other({
    {2, 0, 0, 0},
    {0, 3, 0, 0},
    {0, 0, 4, 0},
    {0, 0, 0, 5}
  });

  EXPECT_EQ(identity * other, other);
  EXPECT_EQ(other * identity, other);
}

TEST(Mat4Test, MultiplicationAssign) {
  Mat4 a({
    {1, 0, 0, 0},
    {0, 2, 0, 0},
    {0, 0, 3, 0},
    {0, 0, 0, 4}
  });

  Mat4 b({
    {2, 0, 0, 0},
    {0, 2, 0, 0},
    {0, 0, 2, 0},
    {0, 0, 0, 2}
  });

  a *= b;

  Mat4 expected({
    {2, 0, 0, 0},
    {0, 4, 0, 0},
    {0, 0, 6, 0},
    {0, 0, 0, 8}
  });

  EXPECT_EQ(a, expected);
}

TEST(Mat4Test, IsApproxEqualMatrices) {
  Mat4 a{
    {{1.0000001, 0.0, 0.0, 0.0},
     {0.0, 1.0, 0.0, 0.0},
     {0.0, 0.0, 1.0, 0.0},
     {0.0, 0.0, 0.0, 1.0}}
  };
  Mat4 b;  // Identity
  EXPECT_TRUE(a.isApprox(b, 1e-6));
}

TEST(Mat4Test, IsApproxDifferentMatrices) {
  Mat4 a = Mat4(1.0);
  Mat4 b = Mat4(2.0);
  EXPECT_FALSE(a.isApprox(b, 1e-6));
}

TEST(Mat4Test, TopLeft3x3ReturnsCorrectSubmatrix) {
  Mat4 mat{
    {{1.0, 2.0, 3.0, 4.0},
     {5.0, 6.0, 7.0, 8.0},
     {9.0, 10.0, 11.0, 12.0},
     {13.0, 14.0, 15.0, 16.0}}
  };
  Mat3 expected{
    {{1.0, 2.0, 3.0},
     {5.0, 6.0, 7.0},
     {9.0, 10.0, 11.0}}
  };
  EXPECT_EQ(mat.topLeft3x3(), expected);
}

TEST(Mat4Test, InverseKnownMatrix) {
  Mat4 mat{
    {{4, 7, 2, 0},
     {3, 6, 1, 0},
     {2, 5, 1, 0},
     {0, 0, 0, 1}}
  };
  Mat4 expectedInverse{
    {{1.0/3.0, 1, -5.0/3.0, 0},
     {-1.0/3.0, 0, 2.0/3.0, 0},
     {1, -2, 1, 0},
     {0, 0, 0, 1}}
  };
  Mat4 inv = mat.inverse();
  EXPECT_TRUE(inv.isApprox(expectedInverse, 1e-6));
}

TEST(Mat4Test, InverseIdentityMatrix) {
  Mat4 identity; 
  Mat4 inv = identity.inverse();
  EXPECT_TRUE(inv.isApprox(identity, 1e-6));
}

TEST(Mat4Test, InverseSingularMatrixThrowsOrReturnsIdentity) {
  Mat4 singular{
    {{1, 2, 3, 4},
     {1, 2, 3, 4},
     {5, 6, 7, 8},
     {9, 10, 11, 12}}
  };

  Mat4 inv = singular.inverse();
  Mat4 product = singular * inv;
  EXPECT_FALSE(product.isApprox(Mat4{}, 1e-6)); 
}

TEST(Mat4Test, InverseMatrixMultiplicationIdentity) {
  Mat4 mat{
    {{3, 0, 2, 0},
     {2, 0, -2, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 1}}
  };
  Mat4 inv = mat.inverse();
  Mat4 product = mat * inv;
  EXPECT_TRUE(product.isApprox(Mat4{}, 1e-6));  
}
