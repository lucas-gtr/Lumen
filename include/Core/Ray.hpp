/**
 * @file Ray.hpp
 * @brief Header file for the Ray structure.
 */
#ifndef CORE_RAY_HPP
#define CORE_RAY_HPP

#include <Eigen/Core>

/**
 * @struct Ray
 * @brief A structure representing a ray in 3D space.
 *
 * A ray is defined by its origin and direction. The direction is normalized between
 * the origin and a given point, which makes the ray direction unit length.
 */
struct Ray {
  Eigen::Vector3d origin    = Eigen::Vector3d::Zero();         ///< The origin of the ray in 3D space.
  Eigen::Vector3d direction = Eigen::Vector3d(0.0, 0.0, -1.0); ///< The normalized direction vector of the ray.

  Ray() = default; ///< Default constructor.

  /**
   * @brief Constructs a Ray from an origin and a point in 3D space.
   *
   * The direction of the ray is calculated as the normalized vector from the origin
   * to the given point.
   *
   * @param origin The origin of the ray.
   * @param point A point in space to define the direction of the ray.
   */
  Ray(const Eigen::Vector3d& origin, const Eigen::Vector3d& point)
      : origin(origin), direction((point - origin).normalized()) {}
};

#endif // CORE_RAY_HPP
