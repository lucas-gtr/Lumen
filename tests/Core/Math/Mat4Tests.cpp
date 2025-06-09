#include <gtest/gtest.h>
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Mat3.hpp"

using namespace lin;

TEST(Mat4dTest, DefaultConstructor) {
  Mat4d mat;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_DOUBLE_EQ(mat(i, j), i == j ? 1.0 : 0.0);
}

TEST(Mat4dTest, ScalarConstructor) {
  Mat4d mat(2.5);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_DOUBLE_EQ(mat(i, j), 2.5);
}

TEST(Mat4dTest, InitializerListConstructor) {
  Mat4d mat({
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

TEST(Mat4dTest, Mat3dConstructor) {
  Mat3d mat3 = {{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }};
  Mat4d mat4(mat3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_DOUBLE_EQ(mat4(i, j), mat3(i, j));

  for (int i = 0; i < 3; ++i)
    EXPECT_DOUBLE_EQ(mat4(i, 3), 0.0);

  for (int j = 0; j < 3; ++j)
    EXPECT_DOUBLE_EQ(mat4(3, j), 0.0);

  EXPECT_DOUBLE_EQ(mat4(3, 3), 1.0);
}

TEST(Mat4dTest, Mat4ConversionConstructor) {
  Mat4f mat4f({
    {1.0f, 2.0f, 3.0f, 4.0f},
    {5.0f, 6.0f, 7.0f, 8.0f},
    {9.0f, 10.0f, 11.0f, 12.0f},
    {13.0f, 14.0f, 15.0f, 16.0f}
  });
  Mat4d mat4d(mat4f);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_DOUBLE_EQ(mat4d(i, j), static_cast<double>(mat4f(i, j)));
}

TEST(Mat4dTest, DataPointer) {
  Mat4d m(1.0);

  m[4] = 2.0;
  m(2, 0) = 3.0;

  const double* data = m.data();
  
  EXPECT_EQ(data, &m(0, 0));
  EXPECT_DOUBLE_EQ(data[4], 2.0); 
  EXPECT_DOUBLE_EQ(data[8], 3.0); 
}

TEST(Mat4dTest, ElementAccessOperators) {
  Mat4d mat;
  mat(2, 3) = 42.0;
  EXPECT_DOUBLE_EQ(mat(2, 3), 42.0);

  mat[7] = 12.0;
  EXPECT_DOUBLE_EQ(mat[7], 12.0);
  EXPECT_DOUBLE_EQ(mat(1, 3), 12.0);
}

TEST(Mat4dTest, EqualityOperators) {
  Mat4d a(1.0);
  Mat4d b(1.0);
  Mat4d c(2.0);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_TRUE(a != c);
  EXPECT_FALSE(a != b);
}

TEST(Mat4dTest, Transpose) {
  Mat4d mat({
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  });

  Mat4d expected({
    {1, 5, 9, 13},
    {2, 6, 10, 14},
    {3, 7, 11, 15},
    {4, 8, 12, 16}
  });

  EXPECT_EQ(mat.transposed(), expected);
}

TEST(Mat4dTest, Multiplication) {
  Mat4d identity;
  Mat4d other({
    {2, 0, 0, 0},
    {0, 3, 0, 0},
    {0, 0, 4, 0},
    {0, 0, 0, 5}
  });

  EXPECT_EQ(identity * other, other);
  EXPECT_EQ(other * identity, other);
}

TEST(Mat4dTest, MultiplicationAssign) {
  Mat4d a({
    {1, 0, 0, 0},
    {0, 2, 0, 0},
    {0, 0, 3, 0},
    {0, 0, 0, 4}
  });

  Mat4d b({
    {2, 0, 0, 0},
    {0, 2, 0, 0},
    {0, 0, 2, 0},
    {0, 0, 0, 2}
  });

  a *= b;

  Mat4d expected({
    {2, 0, 0, 0},
    {0, 4, 0, 0},
    {0, 0, 6, 0},
    {0, 0, 0, 8}
  });

  EXPECT_EQ(a, expected);
}

TEST(Mat4dTest, IsApproxEqualMatrices) {
  Mat4d a{
    {{1.0000001, 0.0, 0.0, 0.0},
     {0.0, 1.0, 0.0, 0.0},
     {0.0, 0.0, 1.0, 0.0},
     {0.0, 0.0, 0.0, 1.0}}
  };
  Mat4d b;  // Identity
  EXPECT_TRUE(a.isApprox(b, 1e-6));
}

TEST(Mat4dTest, IsApproxDifferentMatrices) {
  Mat4d a = Mat4d(1.0);
  Mat4d b = Mat4d(2.0);
  EXPECT_FALSE(a.isApprox(b, 1e-6));
}

TEST(Mat4dTest, TopLeft3x3ReturnsCorrectSubmatrix) {
  Mat4d mat{
    {{1.0, 2.0, 3.0, 4.0},
     {5.0, 6.0, 7.0, 8.0},
     {9.0, 10.0, 11.0, 12.0},
     {13.0, 14.0, 15.0, 16.0}}
  };
  Mat3d expected{
    {{1.0, 2.0, 3.0},
     {5.0, 6.0, 7.0},
     {9.0, 10.0, 11.0}}
  };
  EXPECT_EQ(mat.topLeft3x3(), expected);
}

TEST(Mat4dTest, InverseKnownMatrix) {
  Mat4d mat{
    {{4, 7, 2, 0},
     {3, 6, 1, 0},
     {2, 5, 1, 0},
     {0, 0, 0, 1}}
  };
  Mat4d expectedInverse{
    {{1.0/3.0, 1, -5.0/3.0, 0},
     {-1.0/3.0, 0, 2.0/3.0, 0},
     {1, -2, 1, 0},
     {0, 0, 0, 1}}
  };
  Mat4d inv = mat.inverse();
  EXPECT_TRUE(inv.isApprox(expectedInverse, 1e-6));
}

TEST(Mat4dTest, InverseIdentityMatrix) {
  Mat4d identity; 
  Mat4d inv = identity.inverse();
  EXPECT_TRUE(inv.isApprox(identity, 1e-6));
}

TEST(Mat4dTest, InverseSingularMatrixThrowsOrReturnsIdentity) {
  Mat4d singular{
    {{1, 2, 3, 4},
     {1, 2, 3, 4},
     {5, 6, 7, 8},
     {9, 10, 11, 12}}
  };

  Mat4d inv = singular.inverse();
  Mat4d product = singular * inv;
  EXPECT_FALSE(product.isApprox(Mat4d{}, 1e-6)); 
}

TEST(Mat4dTest, InverseMatrixMultiplicationIdentity) {
  Mat4d mat{
    {{3, 0, 2, 0},
     {2, 0, -2, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 1}}
  };
  Mat4d inv = mat.inverse();
  Mat4d product = mat * inv;
  EXPECT_TRUE(product.isApprox(Mat4d{}, 1e-6));  
}

TEST(Mat4dTest, FromRows) {
  Mat4d mat = Mat4d::FromRows(
    Vec4d(1.0, 2.0, 3.0, 10.0),
    Vec4d(4.0, 5.0, 6.0, 11.0),
    Vec4d(7.0, 8.0, 9.0, 12.0),
    Vec4d(0.0, 0.0, 0.0, 1.0)
  );

  Mat4d expected({
    {1.0, 2.0, 3.0, 10.0},
    {4.0, 5.0, 6.0, 11.0},
    {7.0, 8.0, 9.0, 12.0},
    {0.0, 0.0, 0.0, 1.0}
  });

  EXPECT_TRUE(mat.isApprox(expected, 1e-6));
}

TEST(Mat4dTest, FromColumns) {
  Mat4d mat = Mat4d::FromColumns(
    Vec4d(1.0, 4.0, 7.0, 0.0),
    Vec4d(2.0, 5.0, 8.0, 0.0),
    Vec4d(3.0, 6.0, 9.0, 0.0),
    Vec4d(10.0, 11.0, 12.0, 1.0)
  );

  Mat4d expected({
    {1.0, 2.0, 3.0, 10.0},
    {4.0, 5.0, 6.0, 11.0},
    {7.0, 8.0, 9.0, 12.0},
    {0.0, 0.0, 0.0, 1.0}
  });

  EXPECT_TRUE(mat.isApprox(expected, 1e-6));
}

// TEST(Mat4dTest, Mat4dTest_LookAtMatrix) {
//   Vec3d eye(1.0, 2.0, 5.0);
//   Vec3d center(-1.0, 2.0, 5.0);
//   Vec3d up(0.0, 1.0, 0.0);

//   Mat4d lookAtMatrix = Mat4d::LookAt(eye, center, up);

//   Mat4d expected({
//     {0.7071067811865475, 0.0, -0.7071067811865475, 0.0},
//     {0.0, 1.0, 0.0, 0.0},
//     {0.7071067811865475, 0.0, 0.7071067811865475, 0.0},
//     {-1.4142135623730951, -2.0, -3.5355339059327378, 1.0}
//   });
//   EXPECT_TRUE(lookAtMatrix.isApprox(expected, 1e-6));
// }

TEST(Mat4dTest, Mat4dTest_LookAtMatrix_AutomaticUpVector) {
  Vec3d eye(0.0);
  Vec3d center(0.0, -5.0, 0.0);

  Mat4d lookAtMatrix = Mat4d::LookAt(eye, center);

  Vec3d automaticUp(0.0, 0.0, 1.0); // Default up vector
  Mat4d expected = Mat4d::LookAt(eye, center, automaticUp);

  EXPECT_TRUE(lookAtMatrix.isApprox(expected, 1e-6));
}

TEST(Mat4dTest, Mat4dTest_OrthographicProjection) {
  double left = -1.0, right = 1.0, bottom = -1.0, top = 1.0, near = 0.1, far = 100.0;
  Mat4d orthoMatrix = Mat4d::Orthographic(left, right, bottom, top, near, far);

  Mat4d expected({
    {2.0 / (right - left), 0.0, 0.0, -(right + left) / (right - left)},
    {0.0, 2.0 / (top - bottom), 0.0, -(top + bottom) / (top - bottom)},
    {0.0, 0.0, -2.0 / (far - near), -(far + near) / (far - near)},
    {0.0, 0.0, 0.0, 1.0}
  });
  
  std::cout << "Ortho Matrix: " << orthoMatrix << std::endl;
  std::cout << "Expected: " << expected << std::endl;
  EXPECT_TRUE(orthoMatrix.isApprox(expected, 1e-6));
}

TEST(Mat4dTest, Mat4dTest_PerspectiveProjection) {
  double fovY = 45.0 * DEG_TO_RAD, aspect = 1.0, near = 0.1, far = 100.0;
  Mat4d perspectiveMatrix = Mat4d::Perspective(fovY, aspect, near, far);

  double tanHalfFovY = std::tan(fovY / 2.0);
  
  Mat4d expected({
    {1.0 / (aspect * tanHalfFovY), 0.0, 0.0, 0.0},
    {0.0, 1.0 / tanHalfFovY, 0.0, 0.0},
    {0.0, 0.0, -(far + near) / (far - near), -2 * far * near / (far - near)},
    {0.0, 0.0, -1.0, 0.0}
  });

  EXPECT_TRUE(perspectiveMatrix.isApprox(expected, 1e-6));
}
