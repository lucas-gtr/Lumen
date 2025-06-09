/**
 * @file Vec4.hpp
 * @brief Header file for the Vec4 class, representing a 4D vector.
 */
#ifndef CORE_MATH_VEC4_HPP
#define CORE_MATH_VEC4_HPP

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
 * @brief Generic 4D vector class (Vec4).
 * @tparam T The type of the elements in the vector (e.g., float, double).
 */
template <typename T> struct alignas(lin::VecAlignment<T, 4>::value) Vec4 {
  T x;
  T y;
  T z;
  T w;

  /**
   * @brief Default constructor initializes the vector to (0, 0, 0, 0).
   */
  constexpr Vec4() noexcept : x(0.0), y(0.0), z(0.0), w(0.0) {}

  /**
   * @brief Constructor that initializes the vector with a specific value.
   * @param value The value to initialize all components of the vector.
   */
  constexpr explicit Vec4(T value) noexcept : x(value), y(value), z(value), w(value) {}

  /**
   * @brief Constructor that initializes the vector with specific x, y, z, and w values.
   * @param x The x component of the vector.
   * @param y The y component of the vector.
   * @param z The z component of the vector.
   * @param w The w component of the vector.
   */
  constexpr Vec4(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}

  /**
   * @brief Constructor that initializes the vector from another Vec4 of a different type.
   * @param other The Vec4 of a different type to copy from.
   * @tparam U The type of the other Vec4.
   */
  template <typename U>
  constexpr explicit Vec4(const Vec4<U>& other) noexcept
      : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)), w(static_cast<T>(other.w)) {
  }

  /**
   * @brief Returns a copy of the element at the specified index (0, 1, 2, or 3).
   * @param index The index of the element (0 for x, 1 for y, 2 for z, 3 for w).
   * @return A copy of the element at the specified index.
   * @throws std::out_of_range if the index is not in the range [0, 3].
   */
  constexpr T operator[](int index) const {
    switch(index) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    default:
      throw std::out_of_range("Index out of range for Vec4");
    }
  }

  /**
   * @brief Returns a reference to the element at the specified index (0, 1, 2, or 3).
   * @param index The index of the element (0 for x, 1 for y, 2 for z, 3 for w).
   * @return A reference to the element at the specified index.
   * @throws std::out_of_range if the index is not in the range [0, 3].
   */
  constexpr T& operator[](int index) {
    if(index == 0) {
      return x;
    }
    if(index == 1) {
      return y;
    }
    if(index == 2) {
      return z;
    }
    if(index == 3) {
      return w;
    }
    throw std::out_of_range("Index out of range for Vec4");
  }

  /**
   * @brief Negates the vector, returning a new Vec4 with each component negated.
   * @return A new Vec4 object with negated components.
   */
  constexpr Vec4 operator-() const noexcept { return {-x, -y, -z, -w}; }

  /**
   * @brief Adds two Vec4 vectors element-wise.
   * @param other The Vec4 to add.
   * @return A new Vec4 object that is the sum of this vector and the other vector.
   */
  constexpr Vec4 operator+(const Vec4& other) const noexcept {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
  }

  /**
   * @brief Adds another Vec4 to this vector in place.
   * @param other The Vec4 to add.
   * @return A reference to this vector after the addition.
   */
  Vec4& operator+=(const Vec4& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  /**
   * @brief Subtracts another Vec4 from this vector element-wise.
   * @param other The Vec4 to subtract.
   * @return A new Vec4 object that is the result of the subtraction.
   */
  constexpr Vec4 operator-(const Vec4& other) const noexcept {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
  }

  /**
   * @brief Subtracts another Vec4 from this vector in place.
   * @param other The Vec4 to subtract.
   * @return A reference to this vector after the subtraction.
   */
  Vec4& operator-=(const Vec4& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  /**
   * @brief Multiplies this vector by a scalar, returning a new Vec4.
   * @param scalar The scalar value to multiply with.
   * @return A new Vec4 object that is the result of the multiplication.
   */
  constexpr Vec4 operator*(T scalar) const noexcept { return {x * scalar, y * scalar, z * scalar, w * scalar}; }

  /**
   * @brief Multiplies this vector by a scalar in place.
   * @param scalar The scalar value to multiply with.
   * @return A reference to this vector after the multiplication.
   */
  Vec4& operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  /**
   * @brief Divides this vector by a scalar, returning a new Vec4.
   * @param scalar The scalar value to divide by.
   * @return A new Vec4 object that is the result of dividing each component by the scalar.
   */
  constexpr Vec4 operator/(T scalar) const { return {x / scalar, y / scalar, z / scalar, w / scalar}; }

  /**
   * @brief Divides this vector by a scalar in place.
   * @param scalar The scalar value to divide by.
   * @return A reference to this vector after the division.
   */
  Vec4& operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  }

  /**
   * @brief Compares this vector with another Vec4 for equality.
   * @param other The Vec4 to compare with.
   * @return True if the vectors are equal, false otherwise.
   */
  constexpr bool operator==(const Vec4& other) const noexcept {
    return x == other.x && y == other.y && z == other.z && w == other.w;
  }

  /**
   * @brief Compares this vector with another Vec4 for inequality.
   * @param other The Vec4 to compare with.
   * @return True if the vectors are not equal, false otherwise.
   */
  constexpr bool operator!=(const Vec4& other) const noexcept { return !(*this == other); }

  /**
   * @brief Returns the squared length of the vector.
   * @return The squared length of the vector.
   */
  constexpr T squaredLength() const noexcept { return x * x + y * y + z * z + w * w; }

  /**
   * @brief Returns the length of the vector.
   * @return The length of the vector.
   */
  T length() const noexcept { return std::sqrt(squaredLength()); }

  /**
   * @brief Returns a normalized version of the vector.
   * If the length is zero, returns a zero vector.
   * @return A new Vec4 object that is the normalized version of this vector.
   */
  Vec4 normalized() const noexcept {
    const T len = length();
    return len > 0.0 ? (*this / len) : Vec4(0.0);
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
   * @brief Computes the minimum value of the vector components.
   * @return The minimum value among the x, y, z, and w components.
   */
  constexpr bool isApprox(const Vec4& other, T epsilon) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon && std::fabs(z - other.z) < epsilon &&
           std::fabs(w - other.w) < epsilon;
  }
};

// GCOVR_EXCL_START
/**
 * @brief Overloads the output stream operator for Vec4.
 * @param os The output stream.
 * @param v The Vec4 object to print.
 * @return The output stream after printing the vector.
 * @tparam T The type of the elements in the vector.
 */
template <typename T> inline std::ostream& operator<<(std::ostream& os, const Vec4<T>& v) {
  return os << "Vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}
// GCOVR_EXCL_STOP

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;

} // namespace lin

#endif // CORE_MATH_VEC4_HPP
