#ifndef CORE_MATH_MAT3_HPP
#define CORE_MATH_MAT3_HPP

#include <array>
#include <cmath>
#include <immintrin.h>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "Core/Config.hpp"
#include "Core/Math/Vec3.hpp"

namespace lin {

struct alignas(Align32) Mat3 {
  std::array<std::array<double, 3>, 3> m{};

  constexpr Mat3() noexcept : m{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}} {}

  explicit constexpr Mat3(double value) noexcept
      : m{{{value, value, value}, {value, value, value}, {value, value, value}}} {}

  Mat3(std::initializer_list<std::initializer_list<double>> list) {
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

  constexpr double& operator()(int row, int col) noexcept { return m[row][col]; }
  constexpr double  operator()(int row, int col) const noexcept { return m[row][col]; }

  constexpr double& operator[](int index) noexcept { return m[index / 3][index % 3]; }

  constexpr double operator[](int index) const noexcept { return m[index / 3][index % 3]; }

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
  bool operator!=(const Mat3& other) const noexcept { return !(*this == other); }

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

  double determinant() const noexcept {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  }

  Mat3 inverse() const {
    const double det = determinant();
    if(std::abs(det) < EPSILON) {
      return Mat3{};
    }
    const double invDet = 1.0 / det;

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
  Mat3& operator*=(const Mat3& other) noexcept {
    *this = *this * other;
    return *this;
  }

  bool isApprox(const Mat3& other, double epsilon) const {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        if(std::abs(m[i][j] - other.m[i][j]) > epsilon) {
          return false;
        }
      }
    }
    return true;
  }

  static constexpr Mat3 Identity() noexcept { return Mat3{}; }

  static Mat3 FromRows(const Vec3& row1, const Vec3& row2, const Vec3& row3) {
    return Mat3{{{row1.x, row1.y, row1.z}, {row2.x, row2.y, row2.z}, {row3.x, row3.y, row3.z}}};
  }

  static Mat3 FromColumns(const Vec3& col1, const Vec3& col2, const Vec3& col3) {
    return Mat3{{{col1.x, col2.x, col3.x}, {col1.y, col2.y, col3.y}, {col1.z, col2.z, col3.z}}};
  }
};

inline std::ostream& operator<<(std::ostream& os, const Mat3& mat) {
  return os << "Mat3(\n"
            << "  [" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << "]\n"
            << "  [" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << "]\n"
            << "  [" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << "]\n"
            << ")";
}

} // namespace lin

#endif // CORE_MATH_MAT3_HPP