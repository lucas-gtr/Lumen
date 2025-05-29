#ifndef CORE_MATH_MAT4_HPP
#define CORE_MATH_MAT4_HPP

#include "Core/Math/Mat3.hpp"

namespace lin {

struct Mat4 {
  double m[4][4];

  Mat4() {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        m[i][j] = (i == j) ? 1.0 : 0.0;
      }
    }
  }

  Mat4(double value) {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        m[i][j] = value;
      }
    }
  }

  Mat4(double values[4][4]) {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        m[i][j] = values[i][j];
      }
    }
  }

  Mat4(const Mat3& mat) {
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        m[i][j] = mat.m[i][j];
      }
    }
    m[0][3] = m[1][3] = m[2][3] = 0.0;
    m[3][0] = m[3][1] = m[3][2] = 0.0;
    m[3][3]                     = 1.0;
  }

  double& operator()(int row, int col) { return m[row][col]; }

  double& operator[](int index) { return m[index / 4][index % 4]; }

  const double& operator[](int index) const { return m[index / 4][index % 4]; }

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

  [[nodiscard]] inline Mat4 inverse() const {
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
      // Ici on peut gérer le cas non inversible. Ici on retourne identité ou une copie.
      return Mat4{}; // Retourne matrice nulle (ou tu peux lancer une exception)
    }

    det = 1.0 / det;

    for(int i = 0; i < 16; ++i) {
      inv[i] *= det;
    }

    return inv;
  }

  Mat3 topLeft3x3() const {
    return Mat3{{m[0][0], m[0][1], m[0][2]}, {m[1][0], m[1][1], m[1][2]}, {m[2][0], m[2][1], m[2][2]}};
  }

  bool isApprox(const Mat4& other, double epsilon = 1e-6) const {
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
};

} // namespace lin

#endif // CORE_MATH_MAT4_HPP