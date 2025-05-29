#ifndef CORE_MATH_VEC2_HPP
#define CORE_MATH_VEC2_HPP

#include <cmath>
#include <limits>
#include <stdexcept>

namespace lin {

struct Vec2 {
  double x;
  double y;

  constexpr Vec2() noexcept : x(0.0), y(0.0) {}

  constexpr Vec2(double value) noexcept : x(value), y(value) {}

  constexpr Vec2(double x, double y) noexcept : x(x), y(y) {}

  constexpr double operator[](int index) const {
    return index == 0 ? x : index == 1 ? y : throw std::out_of_range("Index out of range for Vec2");
  }

  constexpr double& operator[](int index) {
    if(index == 0)
      return x;
    if(index == 1)
      return y;
    throw std::out_of_range("Index out of range for Vec2");
  }

  constexpr Vec2 operator-() const noexcept { return Vec2(-x, -y); }

  constexpr Vec2 operator+(const Vec2& other) const noexcept { return Vec2(x + other.x, y + other.y); }

  Vec2& operator+=(const Vec2& other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr Vec2 operator-(const Vec2& other) const noexcept { return Vec2(x - other.x, y - other.y); }

  Vec2& operator-=(const Vec2& other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr Vec2 operator*(double scalar) const noexcept { return Vec2(x * scalar, y * scalar); }

  Vec2& operator*=(double scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  constexpr Vec2 operator/(double scalar) const { return Vec2(x / scalar, y / scalar); }

  Vec2& operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  constexpr bool operator==(const Vec2& other) const noexcept { return x == other.x && y == other.y; }

  constexpr bool operator!=(const Vec2& other) const noexcept { return !(*this == other); }

  constexpr double squaredLength() const noexcept { return x * x + y * y; }

  inline double length() const noexcept { return std::sqrt(squaredLength()); }

  inline Vec2 normalized() const noexcept {
    double len = length();
    return len > 0.0 ? (*this / len) : Vec2(0.0);
  }

  inline void normalize() noexcept {
    double len = length();
    if(len > 0.0)
      *this /= len;
  }

  constexpr bool isApprox(const Vec2& other, double epsilon = 1e-6) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon;
  }
};

} // namespace lin

#endif // CORE_MATH_VEC2_HPP
