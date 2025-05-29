#ifndef CORE_MATH_MAT3_HPP
#define CORE_MATH_MAT3_HPP

#include "Core/Math/Vec3.hpp"

namespace lin {

struct Mat3 {
  double m[3][3];

  Mat3() {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        m[i][j] = (i == j) ? 1.0 : 0.0; // Identity matrix
      }
    }
  }

  Mat3(double value) {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        m[i][j] = value;
      }
    }
  }

  Mat3(double values[3][3]) {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        m[i][j] = values[i][j];
      }
    }
  }

  Mat3(std::initializer_list<std::initializer_list<double>> list) {
    int i = 0;
    for(auto row : list) {
      int j = 0;
      for(auto value : row) {
        m[i][j++] = value;
      }
      ++i;
    }
  }

  double& operator()(int row, int col) { return m[row][col]; }

  Mat3 transpose() const {
    Mat3 result;
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        result.m[j][i] = m[i][j];
      }
    }
    return result;
  }

  Mat3 operator*(const Mat3& other) const {
    Mat3 result;
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j];
      }
    }
    return result;
  }

  Mat3 inverse() const {
    double det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                 m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    if(det == 0.0)
      throw std::runtime_error("Matrix is singular and cannot be inverted.");

    double invDet = 1.0 / det;

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

  static Mat3 Identity() {
    Mat3 identity;
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        identity.m[i][j] = (i == j) ? 1.0 : 0.0;
      }
    }
    return identity;
  }

  static Mat3 FromRows(const Vec3& row1, const Vec3& row2, const Vec3& row3) {
    Mat3 result;
    result.m[0][0] = row1.x;
    result.m[0][1] = row1.y;
    result.m[0][2] = row1.z;
    result.m[1][0] = row2.x;
    result.m[1][1] = row2.y;
    result.m[1][2] = row2.z;
    result.m[2][0] = row3.x;
    result.m[2][1] = row3.y;
    result.m[2][2] = row3.z;
    return result;
  }

  static Mat3 FromColumns(const Vec3& col1, const Vec3& col2, const Vec3& col3) {
    Mat3 result;
    result.m[0][0] = col1.x;
    result.m[0][1] = col2.x;
    result.m[0][2] = col3.x;
    result.m[1][0] = col1.y;
    result.m[1][1] = col2.y;
    result.m[1][2] = col3.y;
    result.m[2][0] = col1.z;
    result.m[2][1] = col2.z;
    result.m[2][2] = col3.z;
    return result;
  }
};
} // namespace lin

#endif // CORE_MATH_MAT3_HPP