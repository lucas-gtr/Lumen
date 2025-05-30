#ifndef CORE_MATH_MAT4_HPP
#define CORE_MATH_MAT4_HPP

#include <array>
#include <cmath>
#include <immintrin.h>
#include <initializer_list>
#include <iostream>

#include "Core/Config.hpp"
#include "Core/Math/Mat3.hpp"
#include "Core/Math/Vec4.hpp"

namespace lin {

struct alignas(Align32) Mat4 {
  std::array<std::array<double, 4>, 4> m{};

  constexpr Mat4()
      : m{{{{1.0, 0.0, 0.0, 0.0}}, {{0.0, 1.0, 0.0, 0.0}}, {{0.0, 0.0, 1.0, 0.0}}, {{0.0, 0.0, 0.0, 1.0}}}} {}

  explicit constexpr Mat4(double value)
      : m{{{{value, value, value, value}},
           {{value, value, value, value}},
           {{value, value, value, value}},
           {{value, value, value, value}}}} {}

  Mat4(std::initializer_list<std::initializer_list<double>> list) {
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

  explicit Mat4(const Mat3& mat) {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        m[i][j] = mat.m[i][j];
      }
    }
    m[0][3] = m[1][3] = m[2][3] = 0.0;
    m[3][0] = m[3][1] = m[3][2] = 0.0;
    m[3][3]                     = 1.0;
  }

  constexpr double& operator()(int row, int col) noexcept { return m[row][col]; }
  constexpr double  operator()(int row, int col) const noexcept { return m[row][col]; }

  constexpr double& operator[](int index) noexcept { return m[index / 4][index % 4]; }

  constexpr double operator[](int index) const noexcept { return m[index / 4][index % 4]; }

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

  bool operator!=(const Mat4& other) const { return !(*this == other); }

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

  Mat4& operator*=(const Mat4& other) {
    *this = *this * other;
    return *this;
  }

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
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

    double det = m[0][0] * inv[0] + m[0][1] * inv[4] + m[0][2] * inv[8] + m[0][3] * inv[12];

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

  Mat3 topLeft3x3() const {
    return Mat3{{m[0][0], m[0][1], m[0][2]}, {m[1][0], m[1][1], m[1][2]}, {m[2][0], m[2][1], m[2][2]}};
  }

  bool isApprox(const Mat4& other, double epsilon) const {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        if(std::abs(m[i][j] - other.m[i][j]) > epsilon) {
          return false;
        }
      }
    }
    return true;
  }

  static Mat4 Identity() {
    Mat4 identity;
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        identity.m[i][j] = (i == j) ? 1.0 : 0.0;
      }
    }
    return identity;
  }

  static Mat4 FromRows(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3) {
    return Mat4{{{row0.x, row0.y, row0.z, row0.w},
                 {row1.x, row1.y, row1.z, row1.w},
                 {row2.x, row2.y, row2.z, row2.w},
                 {row3.x, row3.y, row3.z, row3.w}}};
  }

  static Mat4 FromColumns(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3) {
    return Mat4{{{col0.x, col1.x, col2.x, col3.x},
                 {col0.y, col1.y, col2.y, col3.y},
                 {col0.z, col1.z, col2.z, col3.z},
                 {col0.w, col1.w, col2.w, col3.w}}};
  }
};

inline std::ostream& operator<<(std::ostream& os, const Mat4& mat) {
  return os << "Mat4(\n"
            << "  [" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << ", " << mat.m[0][3] << "]\n"
            << "  [" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << ", " << mat.m[1][3] << "]\n"
            << "  [" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << ", " << mat.m[2][3] << "]\n"
            << "  [" << mat.m[3][0] << ", " << mat.m[3][1] << ", " << mat.m[3][2] << ", " << mat.m[3][3] << "]\n"
            << ")";
}

} // namespace lin

#endif // CORE_MATH_MAT4_HPP