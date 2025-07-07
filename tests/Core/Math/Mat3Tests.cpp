#include <gtest/gtest.h>
#include <linalg/Mat3.hpp>
#include <linalg/Vec3.hpp>

using namespace linalg;

TEST(Mat3dTest, DefaultConstructorCreatesIdentity) {
  Mat3d m;
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

TEST(Mat3dTest, ValueConstructor) {
  Mat3d m(3.0);
  for (int i = 0; i < 9; ++i)
    EXPECT_DOUBLE_EQ(m[i], 3.0);
}

TEST(Mat3dTest, InitializerListConstructorValid) {
  Mat3d m{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 9.0);
}

TEST(Mat3dTest, InitializerListConstructorInvalidSizeThrows) {
  EXPECT_THROW((Mat3d{{ {1, 2}, {3, 4}, {5, 6} }}), std::invalid_argument);
  EXPECT_THROW((Mat3d{{ {1, 2, 3}, {4, 5, 6} }}), std::invalid_argument);
}

TEST(Mat3dTest, Mat3ConversionConstructor) {
  Mat3f m_f{{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f} }};
  Mat3d m_d(m_f);
  EXPECT_DOUBLE_EQ(m_d(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m_d(2, 2), 9.0);
}

TEST(Mat3dTest, DataPointer) {
  Mat3d m(1.0);

  m[4] = 2.0;
  m(2, 0) = 3.0;

  const double* data = m.data();
  
  EXPECT_EQ(data, &m(0, 0));
  EXPECT_DOUBLE_EQ(data[4], 2.0); 
  EXPECT_DOUBLE_EQ(data[6], 3.0); 
}

TEST(Mat3dTest, AccessOperators) {
  Mat3d m(0.0);
  m(1, 2) = 7.0;
  EXPECT_DOUBLE_EQ(m(1, 2), 7.0);
  EXPECT_DOUBLE_EQ(m[5], 7.0); // 1*3 + 2 = 5
}

TEST(Mat3dTest, EqualityOperators) {
  Mat3d a{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3d b = a;
  Mat3d c{{ {1, 2, 0}, {4, 5, 6}, {7, 8, 9} }};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
}

TEST(Mat3dTest, Transpose) {
  Mat3d m{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3d t = m.transposed();
  EXPECT_DOUBLE_EQ(t(0, 1), 4.0);
  EXPECT_DOUBLE_EQ(t(1, 0), 2.0);
  EXPECT_DOUBLE_EQ(t(2, 0), 3.0);
}

TEST(Mat3dTest, Determinant) {
  Mat3d m{{ {6, 1, 1}, {4, -2, 5}, {2, 8, 7} }};
  EXPECT_DOUBLE_EQ(m.determinant(), -306.0);
}

TEST(Mat3dTest, Multiplication) {
  Mat3d a{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3d b = Mat3d::Identity();
  Mat3d c = a * b;
  EXPECT_TRUE(c == a);
}

TEST(Mat3dTest, MultiplicationAssignment) {
  Mat3d a{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat3d b = Mat3d::Identity();
  a *= b;
  Mat3d expected{{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  EXPECT_TRUE(a == expected);
}

TEST(Mat3dTest, IdentityStaticFunction) {
  Mat3d id = Mat3d::Identity();
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_DOUBLE_EQ(id(i, j), i == j ? 1.0 : 0.0);
}

TEST(Mat3dTest, FromRows) {
  Vec3d r1{1.0, 2.0, 3.0};
  Vec3d r2{4.0, 5.0, 6.0};
  Vec3d r3{7.0, 8.0, 9.0};
  Mat3d m = Mat3d::FromRows(r1, r2, r3);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 9.0);
}

TEST(Mat3dTest, FromColumns) {
  Vec3d c1{1.0, 4.0, 7.0};
  Vec3d c2{2.0, 5.0, 8.0};
  Vec3d c3{3.0, 6.0, 9.0};
  Mat3d m = Mat3d::FromColumns(c1, c2, c3);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(1, 0), 4.0);
  EXPECT_DOUBLE_EQ(m(2, 2), 9.0);
}

TEST(Mat3dTest, InverseKnownMatrix) {
  Mat3d mat{
    {{4, 7, 2},
     {3, 6, 1},
     {2, 5, 1}}
  };
  Mat3d expectedInverse{
    {{1.0/3.0, 1, -5.0/3.0},
     {-1.0/3.0, 0, 2.0/3.0},
     {1, -2, 1}}
  };
  Mat3d inv = mat.inverse();
  EXPECT_TRUE(inv.isApprox(expectedInverse, 1e-6));
}

TEST(Mat3dTest, InverseIdentityMatrix) {
  Mat3d identity;
  Mat3d inv = identity.inverse();
  EXPECT_TRUE(inv.isApprox(identity, 1e-6));
}

TEST(Mat3dTest, InverseSingularMatrixThrowsOrReturnsIdentity) {
  Mat3d singular{
    {{1, 2, 3},
     {1, 2, 3},
     {4, 5, 6}}
  };

  Mat3d inv = singular.inverse();
  Mat3d product = singular * inv;
  EXPECT_FALSE(product.isApprox(Mat3d{}, 1e-6)); 
}

TEST(Mat3dTest, InverseMatrixMultiplicationIdentity) {
  Mat3d mat{
    {{3, 0, 2},
     {2, 0, -2},
     {0, 1, 1}}
  };
  Mat3d inv = mat.inverse();
  Mat3d product = mat * inv;
  EXPECT_TRUE(product.isApprox(Mat3d{}, 1e-6)); 
}
