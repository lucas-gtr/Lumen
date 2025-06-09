/**
 * @file Mat4.hpp
 * @brief Header file for the Mat3 class, representing a 3x3 matrix.
 */
#ifndef CORE_MATH_MAT4_HPP
#define CORE_MATH_MAT4_HPP

#include <array>
#include <cmath>
#include <immintrin.h>
#include <initializer_list>
#include <iostream>

#include "Core/Math/Alignment.hpp"
#include "Core/Math/Mat3.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/MathConstants.hpp"

/**
 * @namespace lin
 * @brief Namespace for linear algebra utility functions and classes.
 */
namespace lin {

/**
 * @brief Generic 4x4 matrix class (row-major).
 * @tparam T The type of the elements in the matrix (e.g., float, double).
 */
template <typename T> struct alignas(lin::MatAlignment<T, 4>::value) Mat4 {
  std::array<std::array<T, 4>, 4> m{};

  /**
   * @brief Default constructor initializes the matrix to the identity matrix.
   */
  constexpr Mat4()
      : m{{{{1.0, 0.0, 0.0, 0.0}}, {{0.0, 1.0, 0.0, 0.0}}, {{0.0, 0.0, 1.0, 0.0}}, {{0.0, 0.0, 0.0, 1.0}}}} {}

  /**
   * @brief Constructor that initializes the matrix with a specific value.
   * @param value The value to initialize all elements of the matrix.
   */
  explicit constexpr Mat4(T value)
      : m{{{{value, value, value, value}},
           {{value, value, value, value}},
           {{value, value, value, value}},
           {{value, value, value, value}}}} {}

  /**
   * @brief Constructor that initializes the matrix with an initializer list.
   * @param list An initializer list containing the rows of the matrix.
   */
  Mat4(std::initializer_list<std::initializer_list<T>> list) {
    int i = 0;
    for(auto row : list) {
      int j = 0;
      for(auto value : row) {
        if(i < 4 && j < 4) {
          m[i][j] = value;
        }
        ++j;
      }
      ++i;
    }
  }

  /**
   * @brief Constructor that initializes the matrix from a Mat3.
   * @param mat The Mat3 to convert to a Mat4.
   */
  explicit Mat4(const Mat3<T>& mat) {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        m[i][j] = mat.m[i][j];
      }
    }
    m[0][3] = m[1][3] = m[2][3] = 0.0;
    m[3][0] = m[3][1] = m[3][2] = 0.0;
    m[3][3]                     = 1.0;
  }

  /**
   * @brief Copy constructor.
   * @param other The matrix to copy from.
   */
  template <typename U> explicit Mat4(const Mat4<U>& other) noexcept {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        m[i][j] = static_cast<T>(other.m[i][j]);
      }
    }
  }

  /**
   * @brief Returns a pointer to the underlying data of the matrix.
   * @return A pointer to the first element of the matrix.
   */
  const T* data() const noexcept { return &m[0][0]; }

  /**
   * @brief Returns a reference to the element at the specified row and column.
   * @param row The row index (0, 1, 2, or 3).
   * @param col The column index (0, 1, 2, or 3).
   * @return A reference to the element at the specified position.
   */
  constexpr T& operator()(int row, int col) noexcept { return m[row][col]; }

  /**
   * @brief Returns a copy of the element at the specified row and column.
   * @param row The row index (0, 1, 2, or 3).
   * @param col The column index (0, 1, 2, or 3).
   * @return A copy of the element at the specified position.
   */
  constexpr T operator()(int row, int col) const noexcept { return m[row][col]; }

  /**
   * @brief Returns a reference to the element at the specified index (0-15).
   * @param index The index of the element (0-15).
   * @return A reference to the element at the specified index.
   */
  constexpr T& operator[](int index) noexcept { return m[index / 4][index % 4]; }

  /**
   * @brief Returns a copy of the element at the specified index (0-15).
   * @param index The index of the element (0-15).
   * @return A copy of the element at the specified index.
   */
  constexpr T operator[](int index) const noexcept { return m[index / 4][index % 4]; }

  /**
   * @brief Checks if two matrices are equal.
   * @param other The matrix to compare with.
   * @return True if the matrices are equal, false otherwise.
   */
  bool operator==(const Mat4& other) const {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        if(m[i][j] != other.m[i][j]) {
          return false;
        }
      }
    }
    return true;
  }

  /**
   * @brief Checks if two matrices are not equal.
   * @param other The matrix to compare with.
   * @return True if the matrices are not equal, false otherwise.
   */
  bool operator!=(const Mat4& other) const { return !(*this == other); }

  /**
   * @brief Returns a transposed version of the matrix.
   * @return A new Mat4 object that is the transposed version of this matrix.
   */
  Mat4 transposed() const {
    Mat4 result;

    result.m[0][0] = m[0][0];
    result.m[0][1] = m[1][0];
    result.m[0][2] = m[2][0];
    result.m[0][3] = m[3][0];
    result.m[1][0] = m[0][1];
    result.m[1][1] = m[1][1];
    result.m[1][2] = m[2][1];
    result.m[1][3] = m[3][1];
    result.m[2][0] = m[0][2];
    result.m[2][1] = m[1][2];
    result.m[2][2] = m[2][2];
    result.m[2][3] = m[3][2];
    result.m[3][0] = m[0][3];
    result.m[3][1] = m[1][3];
    result.m[3][2] = m[2][3];
    result.m[3][3] = m[3][3];

    return result;
  }

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  /**
   * @brief Returns the inverse of the matrix.
   * @return A new Mat4 object that is the inverse of this matrix.
   *         If the determinant is zero, returns an identity matrix.
   */
  Mat4 inverse() const {
    Mat4 inv;

    inv[0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] +
             m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];

    inv[1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] -
             m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];

    inv[2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] +
             m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];

    inv[3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] -
             m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];

    inv[4] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] -
             m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];

    inv[5] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] +
             m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];

    inv[6] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] -
             m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];

    inv[7] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] +
             m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];

    inv[8] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] +
             m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];

    inv[9] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] -
             m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];

    inv[10] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] +
              m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];

    inv[11] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] -
              m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];

    inv[12] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] -
              m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    inv[13] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] +
              m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];

    inv[14] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] -
              m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];

    inv[15] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] +
              m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    T det = m[0][0] * inv[0] + m[0][1] * inv[4] + m[0][2] * inv[8] + m[0][3] * inv[12];

    if(det == 0.0) {
      return Mat4{};
    }

    det = 1.0 / det;

    for(int i = 0; i < 16; ++i) {
      inv[i] *= det;
    }

    return inv;
  }
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

  /**
   * @brief Multiplies this matrix by another matrix.
   * @param other The matrix to multiply with.
   * @return A new Mat4 object that is the result of the multiplication.
   */
  Mat4 operator*(const Mat4& other) const {
    Mat4 result;
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        result.m[i][j] =
            m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
      }
    }
    return result;
  }

  /**
   * @brief Multiplies this matrix by another matrix and assigns the result to this matrix.
   * @param other The matrix to multiply with.
   * @return A reference to this matrix after the multiplication.
   */
  Mat4& operator*=(const Mat4& other) {
    *this = *this * other;
    return *this;
  }

  /**
   * @brief Returns a 3x3 matrix containing the top-left 3x3 submatrix of this 4x4 matrix.
   * @return A Mat3 object representing the top-left 3x3 submatrix.
   */
  Mat3<T> topLeft3x3() const {
    return Mat3{{m[0][0], m[0][1], m[0][2]}, {m[1][0], m[1][1], m[1][2]}, {m[2][0], m[2][1], m[2][2]}};
  }

  /**
   * @brief Checks if this matrix is approximately equal to another matrix within a given epsilon.
   * @param other The matrix to compare with.
   * @param epsilon The tolerance for comparison.
   * @return True if the matrices are approximately equal, false otherwise.
   * @tparam T The type of the elements in the matrix.
   */
  bool isApprox(const Mat4& other, T epsilon) const {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        if(std::abs(m[i][j] - other.m[i][j]) > epsilon) {
          return false;
        }
      }
    }
    return true;
  }

  /**
   * @brief Returns the identity matrix.
   * @return A Mat4 object representing the identity matrix.
   */
  static Mat4 Identity() { return Mat4{}; }

  /**
   * @brief Returns a view matrix that transforms coordinates from world space to camera space.
   * @param eye The position of the camera in world space.
   * @param center The point in world space that the camera is looking at.
   * @param up The up direction in world space.
   * @return A Mat4 object representing the view matrix.
   * @tparam T The type of the elements in the matrix.
   */
  static Mat4 LookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up) {
    const Vec3<T> forward  = (center - eye).normalized();
    const Vec3<T> side     = forward.cross(up).normalized();
    const Vec3<T> upVector = side.cross(forward);

    Mat4 lookAtMatrix;
    lookAtMatrix.m[0][0] = side.x;
    lookAtMatrix.m[0][1] = side.y;
    lookAtMatrix.m[0][2] = side.z;
    lookAtMatrix.m[0][3] = -(side.x * eye.x + side.y * eye.y + side.z * eye.z);

    lookAtMatrix.m[1][0] = upVector.x;
    lookAtMatrix.m[1][1] = upVector.y;
    lookAtMatrix.m[1][2] = upVector.z;
    lookAtMatrix.m[1][3] = -(upVector.x * eye.x + upVector.y * eye.y + upVector.z * eye.z);

    lookAtMatrix.m[2][0] = -forward.x;
    lookAtMatrix.m[2][1] = -forward.y;
    lookAtMatrix.m[2][2] = -forward.z;
    lookAtMatrix.m[2][3] = (forward.x * eye.x + forward.y * eye.y + forward.z * eye.z);

    lookAtMatrix.m[3][0] = 0.0;
    lookAtMatrix.m[3][1] = 0.0;
    lookAtMatrix.m[3][2] = 0.0;
    lookAtMatrix.m[3][3] = 1.0;

    return lookAtMatrix;
  }

  /**
   * @brief Returns a view matrix that transforms coordinates from world space to camera space.
   * @param eye The position of the camera in world space.
   * @param center The point in world space that the camera is looking at.
   * @return A Mat4 object representing the view matrix.
   * @tparam T The type of the elements in the matrix.
   */
  static Mat4 LookAt(const Vec3<T>& eye, const Vec3<T>& center) {
    const Vec3<T> forward = (center - eye).normalized();

    const Vec3<T> upWorld = std::abs(forward.y) > 0.99 ? Vec3<T>(0.0, 0.0, 1.0) : Vec3<T>(0.0, 1.0, 0.0);

    return LookAt(eye, center, upWorld);
  }

  /**
   * @brief Returns an orthographic projection matrix.
   * @param left The left clipping plane.
   * @param right The right clipping plane.
   * @param bottom The bottom clipping plane.
   * @param top The top clipping plane.
   * @param near The near clipping plane.
   * @param far The far clipping plane.
   * @return A Mat4 object representing the orthographic projection matrix.
   * @tparam T The type of the elements in the matrix.
   */
  static Mat4 Orthographic(T left, T right, T bottom, T top, T near, T far) {
    Mat4 ortho;
    ortho.m[0][0] = 2 / (right - left);
    ortho.m[1][1] = 2 / (top - bottom);
    ortho.m[2][2] = -2 / (far - near);

    ortho.m[0][3] = -(right + left) / (right - left);
    ortho.m[1][3] = -(top + bottom) / (top - bottom);
    ortho.m[2][3] = -(far + near) / (far - near);

    return ortho;
  }

  /**
   * @brief Returns a perspective projection matrix.
   * @param fovY The vertical field of view in radians.
   * @param aspect The aspect ratio (width/height).
   * @param near The near clipping plane.
   * @param far The far clipping plane.
   * @return A Mat4 object representing the perspective projection matrix.
   * @tparam T The type of the elements in the matrix.
   */
  static Mat4 Perspective(T fovY, T aspect, T near, T far) {
    T    invTanHalfFovY = 1.0 / std::tan(fovY * HALF);
    Mat4 persp;
    persp.m[0][0] = invTanHalfFovY / aspect;
    persp.m[1][1] = invTanHalfFovY;
    persp.m[2][2] = -(far + near) / (far - near);
    persp.m[2][3] = -(2 * far * near) / (far - near);
    persp.m[3][2] = -1.0;
    persp.m[3][3] = 0.0;

    return persp;
  }

  /**
   * @brief Creates a Mat4 from four Vec4 rows.
   * @param row0 The first row of the matrix.
   * @param row1 The second row of the matrix.
   * @param row2 The third row of the matrix.
   * @param row3 The fourth row of the matrix.
   * @return A Mat4 object constructed from the provided rows.
   * @tparam T The type of the elements in the matrix.
   */
  static Mat4 FromRows(const Vec4<T>& row0, const Vec4<T>& row1, const Vec4<T>& row2, const Vec4<T>& row3) {
    return Mat4{{{row0.x, row0.y, row0.z, row0.w},
                 {row1.x, row1.y, row1.z, row1.w},
                 {row2.x, row2.y, row2.z, row2.w},
                 {row3.x, row3.y, row3.z, row3.w}}};
  }

  /**
   * @brief Creates a Mat4 from four Vec4 columns.
   * @param col0 The first column of the matrix.
   * @param col1 The second column of the matrix.
   * @param col2 The third column of the matrix.
   * @param col3 The fourth column of the matrix.
   * @return A Mat4 object constructed from the provided columns.
   * @tparam T The type of the elements in the matrix.
   */
  static Mat4 FromColumns(const Vec4<T>& col0, const Vec4<T>& col1, const Vec4<T>& col2, const Vec4<T>& col3) {
    return Mat4{{{col0.x, col1.x, col2.x, col3.x},
                 {col0.y, col1.y, col2.y, col3.y},
                 {col0.z, col1.z, col2.z, col3.z},
                 {col0.w, col1.w, col2.w, col3.w}}};
  }
};

// GCOVR_EXCL_START
/**
 * @brief Overloaded output operator for Mat4.
 * @param os The output stream.
 * @param mat The Mat4 object to output.
 * @return The output stream after writing the Mat4 object.
 */
template <typename T> inline std::ostream& operator<<(std::ostream& os, const Mat4<T>& mat) {
  return os << "Mat4(\n"
            << "  [" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << ", " << mat.m[0][3] << "]\n"
            << "  [" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << ", " << mat.m[1][3] << "]\n"
            << "  [" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << ", " << mat.m[2][3] << "]\n"
            << "  [" << mat.m[3][0] << ", " << mat.m[3][1] << ", " << mat.m[3][2] << ", " << mat.m[3][3] << "]\n"
            << ")";
}
// GCOVR_EXCL_STOP

using Mat4d = Mat4<double>;
using Mat4f = Mat4<float>;

} // namespace lin

#endif // CORE_MATH_MAT4_HPP