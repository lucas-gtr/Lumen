/**
 * @file Alignment.hpp
 * @brief Defines alignment constants for vector and matrix types.
 */
#ifndef CORE_MATH_ALIGNMENT_HPP
#define CORE_MATH_ALIGNMENT_HPP

#include <cstddef>

namespace lin {

/**
 * @struct VecAlignment
 * @brief Provides alignment constants for vector types.
 * @tparam T The type of the vector elements (e.g., float, double).
 * @tparam N The number of elements in the vector (e.g., 2, 3, 4).
 *
 * This struct is specialized for different types and sizes of vectors to provide
 * the appropriate alignment values. The alignment is typically based on the size
 * of the vector elements and the number of elements in the vector.
 */
template <typename T, int N> struct VecAlignment;

/**
 * @brief Specialization of VecAlignment for float type with 2 elements.
 * @tparam float The type of the vector elements.
 * @tparam 2 The number of elements in the vector.
 */
template <> struct VecAlignment<float, 2> {
  static constexpr size_t value = 16;
};

/**
 * @brief Specialization of VecAlignment for double type with 2 elements.
 * @tparam double The type of the vector elements.
 * @tparam 2 The number of elements in the vector.
 */
template <> struct VecAlignment<double, 2> {
  static constexpr size_t value = 16;
};

/**
 * @brief Specialization of VecAlignment for float type with 3 elements.
 * @tparam float The type of the vector elements.
 * @tparam 3 The number of elements in the vector.
 */
template <> struct VecAlignment<float, 3> {
  static constexpr size_t value = 16;
};

/**
 * @brief Specialization of VecAlignment for double type with 3 elements.
 * @tparam double The type of the vector elements.
 * @tparam 3 The number of elements in the vector.
 */
template <> struct VecAlignment<double, 3> {
  static constexpr size_t value = 32;
};

/**
 * @brief Specialization of VecAlignment for float type with 4 elements.
 * @tparam float The type of the vector elements.
 * @tparam 4 The number of elements in the vector.
 */
template <> struct VecAlignment<float, 4> {
  static constexpr size_t value = 16;
};

/**
 * @brief Specialization of VecAlignment for double type with 4 elements.
 * @tparam double The type of the vector elements.
 * @tparam 4 The number of elements in the vector.
 */
template <> struct VecAlignment<double, 4> {
  static constexpr size_t value = 32;
};

/**
 * @struct MatAlignment
 * @brief Provides alignment constants for matrix types.
 * @tparam T The type of the matrix elements (e.g., float, double).
 * @tparam N The number of rows/columns in the matrix (e.g., 3, 4).
 */
template <typename T, int N> struct MatAlignment;

/**
 * @brief Specialization of MatAlignment for float type with 2 rows/columns.
 * @tparam float The type of the matrix elements.
 * @tparam 2 The number of rows/columns in the matrix.
 */
template <> struct MatAlignment<float, 3> {
  static constexpr size_t value = 16;
};

template <> struct MatAlignment<double, 3> {
  static constexpr size_t value = 32;
};

/**
 * @brief Specialization of MatAlignment for float type with 4 rows/columns.
 * @tparam float The type of the matrix elements.
 * @tparam 4 The number of rows/columns in the matrix.
 */
template <> struct MatAlignment<float, 4> {
  static constexpr size_t value = 16;
};

/**
 * @brief Specialization of MatAlignment for double type with 4 rows/columns.
 * @tparam double The type of the matrix elements.
 * @tparam 4 The number of rows/columns in the matrix.
 */
template <> struct MatAlignment<double, 4> {
  static constexpr size_t value = 32;
};

} // namespace lin

#endif // CORE_MATH_ALIGNMENT_HPP
