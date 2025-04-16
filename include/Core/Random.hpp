/**
 * @file Random.hpp
 * @brief Header file for random number generation functions.
 */
#ifndef CORE_RANDOM_HPP
#define CORE_RANDOM_HPP

#include <Eigen/Core>

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
 * @return A 2D vector (Eigen::Vector2d) representing the random point in the unit disk.
 */
Eigen::Vector2d randomPointInUnitDisk();

#endif // CORE_RANDOM_HPP