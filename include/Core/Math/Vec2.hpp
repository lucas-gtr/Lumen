/**
 * @file Vec2.hpp
 * @brief Header file for the Vec2 class, representing a 2D vector.
 */
#ifndef CORE_MATH_VEC2_HPP
#define CORE_MATH_VEC2_HPP

#include <cmath>
#include <immintrin.h>
#include <iostream>
#include <stdexcept>

#include "Core/Math/Alignment.hpp"

/**
 * @namespace lin
 * @brief Namespace for linear algebra utility functions and classes.
 */
namespace lin {

/**
 * @brief Generic 2D vector class (Vec2).
 * @tparam T The type of the elements in the vector (e.g., float, double).
 */
template <typename T> struct alignas(lin::VecAlignment<T, 2>::value) Vec2 {
  T x;
  T y;

  /**
   * @brief Default constructor initializes the vector to (0, 0).
   */
  constexpr Vec2() noexcept : x(0.0), y(0.0) {}

  /**
   * @brief Constructor that initializes the vector with a specific value.
   * @param value The value to initialize both components of the vector.
   */
  constexpr explicit Vec2(T value) noexcept : x(value), y(value) {}

  /**
   * @brief Constructor that initializes the vector with specific x and y values.
   * @param x The x component of the vector.
   * @param y The y component of the vector.
   */
  constexpr Vec2(T x, T y) noexcept : x(x), y(y) {}

  /**
   * @brief Constructor that initializes the vector from another Vec2 of a different type.
   * @param other The Vec2 of a different type to copy from.
   * @tparam U The type of the other Vec2.
   */
  template <typename U>
  constexpr explicit Vec2(const Vec2<U>& other) noexcept : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

  /**
   * @brief Returns a copy of the element at the specified index (0 or 1).
   * @param index The index of the element (0 for x, 1 for y).
   * @return A copy of the element at the specified index.
   * @throws std::out_of_range if the index is not 0 or 1.
   */
  constexpr T operator[](int index) const {
    switch(index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("Index out of range for Vec2");
    }
  }

  /**
   * @brief Returns a reference to the element at the specified index (0 or 1).
   * @param index The index of the element (0 for x, 1 for y).
   * @return A reference to the element at the specified index.
   * @throws std::out_of_range if the index is not 0 or 1.
   */
  constexpr T& operator[](int index) {
    if(index == 0) {
      return x;
    }
    if(index == 1) {
      return y;
    }
    throw std::out_of_range("Index out of range for Vec2");
  }

  /**
   * @brief Returns the negation of the vector.
   * @return A new Vec2 object with negated components.
   */
  constexpr Vec2 operator-() const noexcept { return {-x, -y}; }

  /**
   * @brief Performs an element-wise addition with another Vec2.
   * @param other The Vec2 to add.
   * @return A new Vec2 object containing the sum of the two vectors.
   */
  constexpr Vec2 operator+(const Vec2& other) const noexcept { return {x + other.x, y + other.y}; }

  /**
   * @brief Adds another Vec2 to this vector.
   * @param other The Vec2 to add.
   * @return A reference to this vector after the addition.
   */
  Vec2& operator+=(const Vec2& other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Performs an element-wise subtraction with another Vec2.
   * @param other The Vec2 to subtract.
   * @return A new Vec2 object containing the difference of the two vectors.
   */
  constexpr Vec2 operator-(const Vec2& other) const noexcept { return {x - other.x, y - other.y}; }

  /**
   * @brief Subtracts another Vec2 from this vector.
   * @param other The Vec2 to subtract.
   * @return A reference to this vector after the subtraction.
   */
  Vec2& operator-=(const Vec2& other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief Performs an element-wise multiplication with a scalar.
   * @param scalar The scalar value to multiply with.
   * @return A new Vec2 object containing the product of each component with the scalar.
   */
  constexpr Vec2 operator*(T scalar) const noexcept { return {x * scalar, y * scalar}; }

  /**
   * @brief Multiplies this vector by a scalar.
   * @param scalar The scalar value to multiply with.
   * @return A reference to this vector after the multiplication.
   */
  Vec2& operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  /**
   * @brief Performs an element-wise division by a scalar.
   * @param scalar The scalar value to divide by.
   * @return A new Vec2 object containing the result of dividing each component by the scalar.
   */
  constexpr Vec2 operator/(T scalar) const { return {x / scalar, y / scalar}; }

  /**
   * @brief Divides this vector by a scalar.
   * @param scalar The scalar value to divide by.
   * @return A reference to this vector after the division.
   */
  Vec2& operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  /**
   * @brief Checks if this vector is equal to another Vec2.
   * @param other The Vec2 to compare with.
   * @return True if the vectors are equal, false otherwise.
   */
  constexpr bool operator==(const Vec2& other) const noexcept { return x == other.x && y == other.y; }

  /**
   * @brief Checks if this vector is not equal to another Vec2.
   * @param other The Vec2 to compare with.
   * @return True if the vectors are not equal, false otherwise.
   */
  constexpr bool operator!=(const Vec2& other) const noexcept { return !(*this == other); }

  /**
   * @brief Computes the squared length of the vector.
   * @return The squared length of the vector.
   */
  constexpr T squaredLength() const noexcept { return x * x + y * y; }

  /**
   * @brief Computes the length of the vector.
   * @return The length of the vector.
   */
  T length() const noexcept { return std::sqrt(squaredLength()); }

  /**
   * @brief Returns a normalized version of the vector.
   * @return A new Vec2 object that is the normalized version of this vector.
   */
  Vec2 normalized() const noexcept {
    const T len = length();
    return len > 0.0 ? (*this / len) : Vec2(0.0);
  }

  /**
   * @brief Normalizes the vector in place.
   * If the length is zero, the vector remains unchanged.
   */
  void normalize() noexcept {
    const T len = length();
    if(len > 0.0) {
      *this /= len;
    }
  }

  /**
   * @brief Checks if this vector is approximately equal to another Vec2 within a given epsilon.
   * @param other The Vec2 to compare with.
   * @param epsilon The tolerance for comparison.
   * @return True if the vectors are approximately equal, false otherwise.
   * @tparam T The type of the elements in the vector.
   */
  constexpr bool isApprox(const Vec2& other, T epsilon) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon;
  }
};

// GCOVR_EXCL_START
/**
 * @brief Overloaded output operator for Vec2.
 * @param os The output stream.
 * @param v The Vec2 object to print.
 * @return The output stream after printing the vector.
 * @tparam T The type of the elements in the vector.
 */
template <typename T> inline std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
  return os << "Vec2(" << v.x << ", " << v.y << ")";
}

// GCOVR_EXCL_STOP

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

} // namespace lin

#endif // CORE_MATH_VEC2_HPP
