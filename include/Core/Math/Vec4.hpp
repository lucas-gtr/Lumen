#ifndef CORE_MATH_VEC4_HPP
#define CORE_MATH_VEC4_HPP

#include <cmath>
#include <limits>
#include <stdexcept>

namespace lin {

struct Vec4 {
  double x;
  double y;
  double z;
  double w;

  constexpr Vec4() noexcept : x(0.0), y(0.0), z(0.0), w(0.0) {}

  constexpr Vec4(double value) noexcept : x(value), y(value), z(value), w(value) {}

  constexpr Vec4(double x, double y, double z, double w) noexcept : x(x), y(y), z(z), w(w) {}

  constexpr double operator[](int index) const {
    return index == 0   ? x
           : index == 1 ? y
           : index == 2 ? z
           : index == 3 ? w
                        : throw std::out_of_range("Index out of range for Vec4");
  }

  constexpr double& operator[](int index) {
    if(index == 0)
      return x;
    if(index == 1)
      return y;
    if(index == 2)
      return z;
    if(index == 3)
      return w;
    throw std::out_of_range("Index out of range for Vec4");
  }

  constexpr Vec4 operator-() const noexcept { return Vec4(-x, -y, -z, -w); }

  constexpr Vec4 operator+(const Vec4& other) const noexcept {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  Vec4& operator+=(const Vec4& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  constexpr Vec4 operator-(const Vec4& other) const noexcept {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  Vec4& operator-=(const Vec4& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  constexpr Vec4 operator*(double scalar) const noexcept {
    return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
  }

  Vec4& operator*=(double scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  constexpr Vec4 operator/(double scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w / scalar); }

  Vec4& operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  }

  constexpr bool operator==(const Vec4& other) const noexcept {
    return x == other.x && y == other.y && z == other.z && w == other.w;
  }

  constexpr bool operator!=(const Vec4& other) const noexcept { return !(*this == other); }

  constexpr double squaredLength() const noexcept { return x * x + y * y + z * z + w * w; }

  inline double length() const noexcept { return std::sqrt(squaredLength()); }

  inline Vec4 normalized() const noexcept {
    double len = length();
    return len > 0.0 ? (*this / len) : Vec4(0.0);
  }

  inline void normalize() noexcept {
    double len = length();
    if(len > 0.0)
      *this /= len;
  }

  constexpr bool isApprox(const Vec4& other, double epsilon = 1e-6) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon && std::fabs(z - other.z) < epsilon &&
           std::fabs(w - other.w) < epsilon;
  }
};

} // namespace lin

#endif // CORE_MATH_VEC4_HPP
