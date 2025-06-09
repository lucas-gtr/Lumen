/**
 * @file Mat3.hpp
 * @brief Header file for the Mat3 class, representing a 3x3 matrix.
 */
#ifndef CORE_MATH_MAT3_HPP
#define CORE_MATH_MAT3_HPP

#include <array>
#include <cmath>
#include <immintrin.h>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "Core/Config.hpp"
#include "Core/Math/Alignment.hpp"
#include "Core/Math/Vec3.hpp"

/**
 * @namespace lin
 * @brief Namespace for linear algebra utility functions and classes.
 */
namespace lin {

/**
 * @brief Generic 3x3 matrix class (row-major).
 * @tparam T The type of the elements in the matrix (e.g., float, double).
 */
template <typename T> struct alignas(lin::MatAlignment<T, 3>::value) Mat3 {
  std::array<std::array<T, 3>, 3> m{};

  /**
   * @brief Default constructor initializes the matrix to the identity matrix.
   */
  constexpr Mat3() noexcept : m{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}} {}

  /**
   * @brief Constructor that initializes the matrix with a specific value.
   * @param value The value to initialize all elements of the matrix.
   */
  explicit constexpr Mat3(T value) noexcept
      : m{{{value, value, value}, {value, value, value}, {value, value, value}}} {}

  /**
   * @brief Constructor that initializes the matrix with an initializer list.
   * @param list An initializer list containing the rows of the matrix.
   * @throws std::invalid_argument if the initializer list does not have 3 rows or if any row does not have 3 elements.
   */
  Mat3(std::initializer_list<std::initializer_list<T>> list) {
    if(list.size() != 3) {
      throw std::invalid_argument("Initializer list must have 3 rows.");
    }
    int i = 0;
    for(const auto& row : list) {
      if(row.size() != 3) {
        throw std::invalid_argument("Each row must have 3 elements.");
      }
      int j = 0;
      for(const auto& value : row) {
        m[i][j] = value;
        ++j;
      }
      ++i;
    }
  }

  /**
   * @brief Copy constructor.
   * @param other The matrix to copy from.
   */
  template <typename U> explicit Mat3(const Mat3<U>& other) noexcept {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
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
   * @param row The row index (0, 1, or 2).
   * @param col The column index (0, 1, or 2).
   * @return A reference to the element at the specified position.
   */
  constexpr T& operator()(int row, int col) noexcept { return m[row][col]; }

  /**
   * @brief Returns a copy of the element at the specified row and column.
   * @param row The row index (0, 1, or 2).
   * @param col The column index (0, 1, or 2).
   * @return A copy of the element at the specified position.
   */
  constexpr T operator()(int row, int col) const noexcept { return m[row][col]; }

  /**
   * @brief Returns a reference to the element at the specified index (0-8).
   * @param index The index of the element (0-8).
   * @return A reference to the element at the specified index.
   */
  constexpr T& operator[](int index) noexcept { return m[index / 3][index % 3]; }

  /**
   * @brief Returns a copy of the element at the specified index (0-8).
   * @param index The index of the element (0-8).
   * @return A copy of the element at the specified index.
   */
  constexpr T operator[](int index) const noexcept { return m[index / 3][index % 3]; }

  /**
   * @brief Checks if two matrices are equal.
   * @param other The matrix to compare with.
   * @return True if the matrices are equal, false otherwise.
   */
  bool operator==(const Mat3& other) const noexcept {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
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
  bool operator!=(const Mat3& other) const noexcept { return !(*this == other); }

  /**
   * @brief Returns a transposed version of the matrix.
   * @return A new Mat3 object that is the transpose of the current matrix.
   */
  Mat3 transposed() const noexcept {
    Mat3 result;
    result.m[0][0] = m[0][0];
    result.m[0][1] = m[1][0];
    result.m[0][2] = m[2][0];
    result.m[1][0] = m[0][1];
    result.m[1][1] = m[1][1];
    result.m[1][2] = m[2][1];
    result.m[2][0] = m[0][2];
    result.m[2][1] = m[1][2];
    result.m[2][2] = m[2][2];
    return result;
  }

  /**
   * @brief Returns the determinant of the matrix.
   * @return The determinant of the matrix.
   */
  T determinant() const noexcept {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  }

  /**
   * @brief Returns the inverse of the matrix.
   * @return A new Mat3 object that is the inverse of the current matrix.
   *         If the determinant is zero, returns an identity matrix.
   */
  Mat3 inverse() const {
    const T det = determinant();
    if(std::abs(det) < EPSILON) {
      return Mat3{};
    }
    const T invDet = 1.0 / det;

    Mat3 inv;
    inv.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    inv.m[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * invDet;
    inv.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

    inv.m[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * invDet;
    inv.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    inv.m[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * invDet;

    inv.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    inv.m[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * invDet;
    inv.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return inv;
  }

  /**
   * @brief Returns the product of this matrix with another matrix.
   * @param other The matrix to multiply with.
   * @return A new Mat3 object that is the result of the multiplication.
   */
  Mat3 operator*(const Mat3& other) const noexcept {
    Mat3 result;
    for(int i = 0; i < 3; ++i) {
      const auto& row = m[i];
      for(int j = 0; j < 3; ++j) {
        result.m[i][j] = row[0] * other.m[0][j] + row[1] * other.m[1][j] + row[2] * other.m[2][j];
      }
    }
    return result;
  }

  /**
   * @brief Multiplies this matrix by another matrix and returns the result.
   * @param other The matrix to multiply with.
   * @return A reference to this matrix after multiplication.
   */
  Mat3& operator*=(const Mat3& other) noexcept {
    *this = *this * other;
    return *this;
  }

  /**
   * @brief Checks if this matrix is approximately equal to another matrix within a given epsilon.
   * @param other The matrix to compare with.
   * @param epsilon The tolerance for comparison.
   * @return True if the matrices are approximately equal, false otherwise.
   * @tparam T The type of the elements in the matrix.
   */
  bool isApprox(const Mat3& other, T epsilon) const {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        if(std::abs(m[i][j] - other.m[i][j]) > epsilon) {
          return false;
        }
      }
    }
    return true;
  }

  /**
   * @brief Returns the identity matrix.
   * @return A Mat3 object representing the identity matrix.
   */
  static constexpr Mat3 Identity() noexcept { return Mat3{}; }

  /**
   * @brief Creates a Mat3 from three rows.
   * @param row1 The first row of the matrix.
   * @param row2 The second row of the matrix.
   * @param row3 The third row of the matrix.
   * @return A Mat3 object constructed from the given rows.
   * @tparam T The type of the elements in the matrix.
   * @note The rows are expected to be of type Vec3<T>.
   */
  static Mat3 FromRows(const Vec3<T>& row1, const Vec3<T>& row2, const Vec3<T>& row3) {
    return Mat3{{{row1.x, row1.y, row1.z}, {row2.x, row2.y, row2.z}, {row3.x, row3.y, row3.z}}};
  }

  /**
   * @brief Creates a Mat3 from three columns.
   * @param col1 The first column of the matrix.
   * @param col2 The second column of the matrix.
   * @param col3 The third column of the matrix.
   * @return A Mat3 object constructed from the given columns.
   * @tparam T The type of the elements in the matrix.
   * @note The columns are expected to be of type Vec3<T>.
   */
  static Mat3 FromColumns(const Vec3<T>& col1, const Vec3<T>& col2, const Vec3<T>& col3) {
    return Mat3{{{col1.x, col2.x, col3.x}, {col1.y, col2.y, col3.y}, {col1.z, col2.z, col3.z}}};
  }
};

// GCOVR_EXCL_START
/**
 * @brief Overloads the output stream operator for Mat3.
 * @param os The output stream.
 * @param mat The Mat3 object to print.
 * @return The output stream after printing the matrix.
 * @tparam T The type of the elements in the matrix.
 */
template <typename T> inline std::ostream& operator<<(std::ostream& os, const Mat3<T>& mat) {
  return os << "Mat3(\n"
            << "  [" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << "]\n"
            << "  [" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << "]\n"
            << "  [" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << "]\n"
            << ")";
}
// GCOVR_EXCL_STOP

using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;

} // namespace lin

#endif // CORE_MATH_MAT3_HPP