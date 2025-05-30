#ifndef CORE_MATH_LIN_HPP
#define CORE_MATH_LIN_HPP

#include <cmath>

#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/Vec4.hpp"

namespace lin {

inline Vec3 toVec3(const Vec4& vec) { return {vec.x, vec.y, vec.z}; }
inline Vec4 toVec4(const Vec3& vec) { return {vec.x, vec.y, vec.z, 1.0}; }

inline double dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
inline double dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline double dot(const Vec4& a, const Vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

inline Vec2 cwiseMin(const Vec2& a, const Vec2& b) { return {std::fmin(a.x, b.x), std::fmin(a.y, b.y)}; }

inline Vec3 cwiseMin(const Vec3& a, const Vec3& b) {
  return {std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z)};
}

inline Vec4 cwiseMin(const Vec4& a, const Vec4& b) {
  return {std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z), std::fmin(a.w, b.w)};
}

inline Vec2 cwiseMax(const Vec2& a, const Vec2& b) { return {std::fmax(a.x, b.x), std::fmax(a.y, b.y)}; }

inline Vec3 cwiseMax(const Vec3& a, const Vec3& b) {
  return {std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z)};
}

inline Vec4 cwiseMax(const Vec4& a, const Vec4& b) {
  return {std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z), std::fmax(a.w, b.w)};
}

inline Vec2 cwiseProduct(const Vec2& a, const Vec2& b) { return {a.x * b.x, a.y * b.y}; }

inline Vec3 cwiseProduct(const Vec3& a, const Vec3& b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }

inline Vec4 cwiseProduct(const Vec4& a, const Vec4& b) { return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; }

inline Vec2 operator*(double scalar, const Vec2& vec) { return {vec.x * scalar, vec.y * scalar}; }
inline Vec3 operator*(double scalar, const Vec3& vec) { return {vec.x * scalar, vec.y * scalar, vec.z * scalar}; }
inline Vec4 operator*(double scalar, const Vec4& vec) {
  return {vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar};
}

inline Vec3 operator*(const Mat3& mat, const Vec3& vec) {
  return {mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z,
          mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z,
          mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z};
}

inline Vec4 operator*(const Mat4& mat, const Vec4& vec) {
  return {mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w,
          mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w,
          mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w,
          mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w};
}

inline Mat3 getRotationMatrix(double x_angle, double y_angle, double z_angle) {
  const Mat3 rx =
      Mat3({{1, 0, 0}, {0, std::cos(x_angle), -std::sin(x_angle)}, {0, std::sin(x_angle), std::cos(x_angle)}});

  const Mat3 ry =
      Mat3({{std::cos(y_angle), 0, std::sin(y_angle)}, {0, 1, 0}, {-std::sin(y_angle), 0, std::cos(y_angle)}});

  const Mat3 rz =
      Mat3({{std::cos(z_angle), -std::sin(z_angle), 0}, {std::sin(z_angle), std::cos(z_angle), 0}, {0, 0, 1}});

  return rz * ry * rx;
}

} // namespace lin

#endif // CORE_MATH_LIN_HPP
