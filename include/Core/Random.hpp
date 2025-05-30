/**
 * @file Random.hpp
 * @brief Header file for random number generation functions.
 */
#ifndef CORE_RANDOM_HPP
#define CORE_RANDOM_HPP

#include "Core/Math/Vec2.hpp"

/**
 * @brief Generates a random point inside the unit disk.
 *
 * This function generates a random 2D point where both coordinates lie within
 * the unit disk, i.e., the point satisfies the condition:
 * \f$ x^2 + y^2 \leq 1 \f$.
 *
 * The point is generated using a random angle and radius within the unit disk,
 * ensuring uniform distribution within the disk.
 *
 * @return A 2D vector representing the random point in the unit disk.
 */
lin::Vec2 randomPointInUnitDisk();

/**
 * @brief Generates a random number uniformly distributed in the range [0, 1).
 *
 * This function uses a random number generator to produce a double-precision
 * floating-point number uniformly distributed in the range [0, 1).
 *
 * @return A double representing the random number in the range [0, 1).
 */
double randomUniform01();

#endif // CORE_RANDOM_HPP