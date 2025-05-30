#ifndef CORE_MATH_VEC2_HPP
#define CORE_MATH_VEC2_HPP

#include <cmath>
#include <immintrin.h>
#include <iostream>
#include <stdexcept>

#include "Core/Config.hpp"

namespace lin {

struct alignas(Align16) Vec2 {
  double x;
  double y;

  constexpr Vec2() noexcept : x(0.0), y(0.0) {}

  constexpr explicit Vec2(double value) noexcept : x(value), y(value) {}

  constexpr Vec2(double x, double y) noexcept : x(x), y(y) {}

  constexpr double operator[](int index) const {
    switch(index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("Index out of range for Vec2");
    }
  }

  constexpr double& operator[](int index) {
    if(index == 0) {
      return x;
    }
    if(index == 1) {
      return y;
    }
    throw std::out_of_range("Index out of range for Vec2");
  }

  constexpr Vec2 operator-() const noexcept { return {-x, -y}; }

  constexpr Vec2 operator+(const Vec2& other) const noexcept { return {x + other.x, y + other.y}; }

  Vec2& operator+=(const Vec2& other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr Vec2 operator-(const Vec2& other) const noexcept { return {x - other.x, y - other.y}; }

  Vec2& operator-=(const Vec2& other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr Vec2 operator*(double scalar) const noexcept { return {x * scalar, y * scalar}; }

  Vec2& operator*=(double scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  constexpr Vec2 operator/(double scalar) const { return {x / scalar, y / scalar}; }

  Vec2& operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  constexpr bool operator==(const Vec2& other) const noexcept { return x == other.x && y == other.y; }

  constexpr bool operator!=(const Vec2& other) const noexcept { return !(*this == other); }

  constexpr double squaredLength() const noexcept { return x * x + y * y; }

  double length() const noexcept { return std::sqrt(squaredLength()); }

  Vec2 normalized() const noexcept {
    const double len = length();
    return len > 0.0 ? (*this / len) : Vec2(0.0);
  }

  void normalize() noexcept {
    const double len = length();
    if(len > 0.0) {
      *this /= len;
    }
  }

  constexpr bool isApprox(const Vec2& other, double epsilon) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Vec2& v) { return os << "Vec2(" << v.x << ", " << v.y << ")"; }

} // namespace lin

#endif // CORE_MATH_VEC2_HPP
