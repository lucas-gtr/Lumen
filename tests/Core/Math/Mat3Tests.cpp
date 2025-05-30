#include <gtest/gtest.h>
#include "Core/Math/Mat3.hpp"
#include "Core/Math/Vec3.hpp"

using namespace lin;

TEST(Mat3Test, DefaultConstructorCreatesIdentity) {
  Mat3 m;
  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(1, 1), 1.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 1.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 0.0);
  EXPECT_DOUBLE_EQ(m(0, 2), 0.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 0.0);
  EXPECT_DOUBLE_EQ(m(1, 2), 0.0);
  EXPECT_DOUBLE_EQ(m(2, 0), 0.0);
  EXPECT_DOUBLE_EQ(m(2, 1), 0.0);
}

TEST(Mat3Test, ValueConstructor) {
  Mat3 m(3.0);
  for (int i = 0; i < 9; ++i)
    EXPECT_DOUBLE_EQ(m[i], 3.0);
}

TEST(Mat3Test, InitializerListConstructorValid) {
  Mat3 m{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 9.0);
}

TEST(Mat3Test, InitializerListConstructorInvalidSizeThrows) {
  EXPECT_THROW((Mat3{{ {1, 2}, {3, 4}, {5, 6} }}), std::invalid_argument);
  EXPECT_THROW((Mat3{{ {1, 2, 3}, {4, 5, 6} }}), std::invalid_argument);
}

TEST(Mat3Test, AccessOperators) {
  Mat3 m(0.0);
  m(1, 2) = 7.0;
  EXPECT_DOUBLE_EQ(m(1, 2), 7.0);
  EXPECT_DOUBLE_EQ(m[5], 7.0); // 1*3 + 2 = 5
}

TEST(Mat3Test, EqualityOperators) {
  Mat3 a{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3 b = a;
  Mat3 c{{ {1, 2, 0}, {4, 5, 6}, {7, 8, 9} }};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TEST(Mat3Test, Transpose) {
  Mat3 m{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3 t = m.transposed();
  EXPECT_DOUBLE_EQ(t(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(t(1, 0), 2.0);
  EXPECT_DOUBLE_EQ(t(2, 0), 3.0);
}

TEST(Mat3Test, Determinant) {
  Mat3 m{{ {6, 1, 1}, {4, -2, 5}, {2, 8, 7} }};
  EXPECT_DOUBLE_EQ(m.determinant(), -306.0);
}

TEST(Mat3Test, Multiplication) {
  Mat3 a{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3 b = Mat3::Identity();
  Mat3 c = a * b;
  EXPECT_TRUE(c == a);
}

TEST(Mat3Test, MultiplicationAssignment) {
  Mat3 a{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3 b = Mat3::Identity();
  a *= b;
  Mat3 expected{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  EXPECT_TRUE(a == expected);
}

TEST(Mat3Test, IdentityStaticFunction) {
  Mat3 id = Mat3::Identity();
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_DOUBLE_EQ(id(i, j), i == j ? 1.0 : 0.0);
}

TEST(Mat3Test, FromRows) {
  Vec3 r1{1.0, 2.0, 3.0};
  Vec3 r2{4.0, 5.0, 6.0};
  Vec3 r3{7.0, 8.0, 9.0};
  Mat3 m = Mat3::FromRows(r1, r2, r3);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 9.0);
}

TEST(Mat3Test, FromColumns) {
  Vec3 c1{1.0, 4.0, 7.0};
  Vec3 c2{2.0, 5.0, 8.0};
  Vec3 c3{3.0, 6.0, 9.0};
  Mat3 m = Mat3::FromColumns(c1, c2, c3);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 4.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 9.0);
}

TEST(Mat3Test, InverseKnownMatrix) {
  Mat3 mat{
    {{4, 7, 2},
     {3, 6, 1},
     {2, 5, 1}}
  };
  Mat3 expectedInverse{
    {{1.0/3.0, 1, -5.0/3.0},
     {-1.0/3.0, 0, 2.0/3.0},
     {1, -2, 1}}
  };
  Mat3 inv = mat.inverse();
  EXPECT_TRUE(inv.isApprox(expectedInverse, 1e-6));
}

TEST(Mat3Test, InverseIdentityMatrix) {
  Mat3 identity;
  Mat3 inv = identity.inverse();
  EXPECT_TRUE(inv.isApprox(identity, 1e-6));
}

TEST(Mat3Test, InverseSingularMatrixThrowsOrReturnsIdentity) {
  Mat3 singular{
    {{1, 2, 3},
     {1, 2, 3},
     {4, 5, 6}}
  };

  Mat3 inv = singular.inverse();
  Mat3 product = singular * inv;
  EXPECT_FALSE(product.isApprox(Mat3{}, 1e-6)); 
}

TEST(Mat3Test, InverseMatrixMultiplicationIdentity) {
  Mat3 mat{
    {{3, 0, 2},
     {2, 0, -2},
     {0, 1, 1}}
  };
  Mat3 inv = mat.inverse();
  Mat3 product = mat * inv;
  EXPECT_TRUE(product.isApprox(Mat3{}, 1e-6)); 
}
