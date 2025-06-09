/**
 * @file Vec3.hpp
 * @brief Header file for the Vec3 class, representing a 3D vector.
 */
#ifndef CORE_MATH_VEC3_HPP
#define CORE_MATH_VEC3_HPP

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
 * @brief Generic 3D vector class (Vec3).
 * @tparam T The type of the elements in the vector (e.g., float, double).
 */
template <typename T> struct alignas(lin::VecAlignment<T, 3>::value) Vec3 {
  T x;
  T y;
  T z;

  /**
   * @brief Default constructor initializes the vector to (0, 0, 0).
   */
  constexpr Vec3() noexcept : x(0.0), y(0.0), z(0.0) {}

  /**
   * @brief Constructor that initializes the vector with a specific value.
   */
  constexpr explicit Vec3(T value) noexcept : x(value), y(value), z(value) {}

  /**
   * @brief Constructor that initializes the vector with specific x, y, and z values.
   * @param x The x component of the vector.
   * @param y The y component of the vector.
   * @param z The z component of the vector.
   */
  constexpr Vec3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}

  /**
   * @brief Constructor that initializes the vector from another Vec3 of a different type.
   * @param other The Vec3 of a different type to copy from.
   * @tparam U The type of the other Vec3.
   */
  template <typename U>
  constexpr explicit Vec3(const Vec3<U>& other) noexcept
      : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)) {}

  /**
   * @brief Returns a copy of the element at the specified index (0, 1, or 2).
   * @param index The index of the element (0 for x, 1 for y, 2 for z).
   * @return A copy of the element at the specified index.
   * @throws std::out_of_range if the index is not 0, 1, or 2.
   */
  constexpr T operator[](int index) const {
    switch(index) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      throw std::out_of_range("Index out of range for Vec3");
    }
  }

  /**
   * @brief Returns a reference to the element at the specified index (0, 1, or 2).
   * @param index The index of the element (0 for x, 1 for y, 2 for z).
   * @return A reference to the element at the specified index.
   * @throws std::out_of_range if the index is not 0, 1, or 2.
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
    throw std::out_of_range("Index out of range for Vec3");
  }

  /**
   * @brief Negates the vector, returning a new Vec3 with each component negated.
   * @return A new Vec3 object with negated components.
   */
  constexpr Vec3 operator-() const noexcept { return {-x, -y, -z}; }

  /**
   * @brief Performs an element-wise addition of this vector and another Vec3.
   * @param other The Vec3 to add.
   * @return A new Vec3 object that is the sum of this vector and the other vector.
   */
  constexpr Vec3 operator+(const Vec3& other) const noexcept { return {x + other.x, y + other.y, z + other.z}; }

  /**
   * @brief Adds another Vec3 to this vector in place.
   * @param other The Vec3 to add.
   * @return A reference to this vector after the addition.
   */
  Vec3& operator+=(const Vec3& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  /**
   * @brief Performs an element-wise subtraction of this vector and another Vec3.
   * @param other The Vec3 to subtract.
   * @return A new Vec3 object that is the difference of this vector and the other vector.
   */
  constexpr Vec3 operator-(const Vec3& other) const noexcept { return {x - other.x, y - other.y, z - other.z}; }

  /**
   * @brief Subtracts another Vec3 from this vector in place.
   * @param other The Vec3 to subtract.
   * @return A reference to this vector after the subtraction.
   */
  Vec3& operator-=(const Vec3& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  /**
   * @brief Multiplies this vector by a scalar, returning a new Vec3.
   * @param scalar The scalar value to multiply with.
   * @return A new Vec3 object containing the product of each component with the scalar.
   */
  constexpr Vec3 operator*(T scalar) const noexcept { return {x * scalar, y * scalar, z * scalar}; }

  /**
   * @brief Multiplies this vector by a scalar in place.
   * @param scalar The scalar value to multiply with.
   * @return A reference to this vector after the multiplication.
   */
  Vec3& operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  /**
   * @brief Divides this vector by a scalar, returning a new Vec3.
   * @param scalar The scalar value to divide by.
   * @return A new Vec3 object containing the result of dividing each component by the scalar.
   */
  constexpr Vec3 operator/(T scalar) const {
    return {x / scalar, y / scalar, z / scalar}; // division by zero is undefined
  }

  /**
   * @brief Divides this vector by a scalar in place.
   * @param scalar The scalar value to divide by.
   * @return A reference to this vector after the division.
   */
  Vec3& operator/=(T scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  /**
   * @brief Checks if this vector is equal to another Vec3.
   * @param other The Vec3 to compare with.
   * @return True if the vectors are equal, false otherwise.
   */
  constexpr bool operator==(const Vec3& other) const noexcept { return x == other.x && y == other.y && z == other.z; }

  /**
   * @brief Checks if this vector is not equal to another Vec3.
   * @param other The Vec3 to compare with.
   * @return True if the vectors are not equal, false otherwise.
   */
  constexpr bool operator!=(const Vec3& other) const noexcept { return !(*this == other); }

  /**
   * @brief Computes the squared length of the vector.
   * @return The squared length of the vector.
   */
  constexpr T squaredLength() const noexcept { return x * x + y * y + z * z; }

  /**
   * @brief Computes the length of the vector.
   * @return The length of the vector.
   */
  T length() const noexcept { return std::sqrt(squaredLength()); }

  /**
   * @brief Returns a normalized version of the vector.
   * @return A new Vec3 object that is the normalized version of this vector.
   */
  Vec3 normalized() const noexcept {
    const T len = length();
    return len > 0.0 ? (*this / len) : Vec3(0.0);
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
   * @brief Computes the cross product of this vector and another Vec3.
   * @param other The Vec3 to compute the cross product with.
   * @return A new Vec3 object that is the result of the cross product.
   */
  constexpr Vec3 cross(const Vec3& other) const noexcept {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
  }

  /**
   * @brief Computes an element-wise inverse of the vector.
   * @return A new Vec3 object where each component is the inverse of the corresponding component of this vector.
   */
  constexpr Vec3 cwiseInverse() const noexcept { return {1.0 / x, 1.0 / y, 1.0 / z}; }

  /**
   * @brief Computes the minimum value of the vector components.
   * @return The minimum value among the x, y, and z components.
   */
  constexpr T minValue() const noexcept { return std::fmin(std::fmin(x, y), z); }

  /**
   * @brief Computes the maximum value of the vector components.
   * @return The maximum value among the x, y, and z components.
   */
  constexpr T maxValue() const noexcept { return std::fmax(std::fmax(x, y), z); }

  /**
   * @brief Checks if this vector is approximately equal to another Vec3 within a given epsilon.
   * @param other The Vec3 to compare with.
   * @param epsilon The tolerance for comparison.
   * @return True if the vectors are approximately equal, false otherwise.
   */
  constexpr bool isApprox(const Vec3& other, T epsilon) const noexcept {
    return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon && std::fabs(z - other.z) < epsilon;
  }
};

// GCOVR_EXCL_START
/**
 * @brief Overloads the output stream operator for Vec3.
 * @param os The output stream.
 * @param v The Vec3 object to print.
 * @return The output stream after printing the vector.
 * @tparam T The type of the elements in the vector.
 */
template <typename T> inline std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
  return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

// GCOVR_EXCL_STOP

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

} // namespace lin

#endif // CORE_MATH_VEC3_HPP
