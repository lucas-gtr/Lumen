/**
 * @file Ray.hpp
 * @brief Header file for the Ray structure.
 */
#ifndef CORE_RAY_HPP
#define CORE_RAY_HPP

#include "Core/Math/Vec3.hpp"

/**
 * @struct Ray
 * @brief A structure representing a ray in 3D space.
 *
 * A ray is defined by its origin and direction. The direction is normalized between
 * the origin and a given point, which makes the ray direction unit length.
 */
struct Ray {
  lin::Vec3d origin    = {0, 0, 0};
  lin::Vec3d direction = {0, 0, -1};

  /**
   * @brief Constructs a Ray from two points.
   * @param from The starting point of the ray.
   * @param to The ending point of the ray.
   * @return A Ray object with the origin at 'from' and direction normalized from 'from' to 'to'.
   */
  static Ray FromPoint(const lin::Vec3d& from, const lin::Vec3d& to) { return Ray{from, (to - from).normalized()}; }

  /**
   * @brief Constructs a Ray from an origin and a direction vector.
   * @param origin The origin point of the ray.
   * @param dir The direction vector of the ray, which will be normalized.
   * @return A Ray object with the specified origin and normalized direction.
   */
  static Ray FromDirection(const lin::Vec3d& origin, const lin::Vec3d& dir) { return Ray{origin, dir.normalized()}; }

private:
  Ray(const lin::Vec3d& o, const lin::Vec3d& d) : origin(o), direction(d) {}
};

#endif // CORE_RAY_HPP
