#ifndef CORE_MATH_VEC3_HPP
#define CORE_MATH_VEC3_HPP

#include <cmath>
#include <limits>
#include <stdexcept>

namespace lin {

struct Vec3 {
  double x;
  double y;
  double z;

  constexpr Vec3() noexcept : x(0.0), y(0.0), z(0.0) {}

  constexpr Vec3(double value) noexcept : x(value), y(value), z(value) {}

  constexpr Vec3(double x, double y, double z) noexcept : x(x), y(y), z(z) {}

  constexpr double operator[](int index) const {
    return index == 0 ? x : index == 1 ? y : index == 2 ? z : throw std::out_of_range("Index out of range for Vec3");
  }

  constexpr double& operator[](int index) {
    if(index == 0)
      return x;
    if(index == 1)
      return y;
    if(index == 2)
      return z;
    throw std::out_of_range("Index out of range for Vec3");
  }

  constexpr Vec3 operator-() const noexcept { return Vec3(-x, -y, -z); }

  constexpr Vec3 operator+(const Vec3& other) const noexcept { return Vec3(x + other.x, y + other.y, z + other.z); }

  Vec3& operator+=(const Vec3& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  constexpr Vec3 operator-(const Vec3& other) const noexcept { return Vec3(x - other.x, y - other.y, z - other.z); }

  Vec3& operator-=(const Vec3& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  constexpr Vec3 operator*(double scalar) const noexcept { return Vec3(x * scalar, y * scalar, z * scalar); }

  Vec3& operator*=(double scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  constexpr Vec3 operator/(double scalar) const {
    return Vec3(x / scalar, y / scalar, z / scalar); // division by zero is undefined
  }

  Vec3& operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  constexpr bool operator==(const Vec3& other) const noexcept { return x == other.x && y == other.y && z == other.z; }

  constexpr bool operator!=(const Vec3& other) const noexcept { return !(*this == other); }

  constexpr double squaredLength() const noexcept { return x * x + y * y + z * z; }

  inline double length() const noexcept { return std::sqrt(squaredLength()); }

  inline Vec3 normalized() const noexcept {
    double len = length();
    return len > 0.0 ? (*this / len) : Vec3(0.0);
  }

  inline void normalize() noexcept {
    double len = length();
    if(len > 0.0) {
      *this /= len;
    }
  }

  constexpr Vec3 cross(const Vec3& other) const noexcept {
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
  }

  constexpr Vec3 cwiseInverse() const noexcept { return Vec3(1.0 / x, 1.0 / y, 1.0 / z); }

  constexpr double minValue() const noexcept { return std::fmin(std::fmin(x, y), z); }

  constexpr double maxValue() const noexcept { return std::fmax(std::fmax(x, y), z); }

  constexpr bool isApprox(const Vec3& other, double epsilon = 1e-6) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon && std::fabs(z - other.z) < epsilon;
  }
};

} // namespace lin

#endif // CORE_MATH_VEC3_HPP
