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
  lin::Vec3 origin    = {0, 0, 0};
  lin::Vec3 direction = {0, 0, -1};

  static Ray FromPoint(const lin::Vec3& from, const lin::Vec3& to) { return Ray{from, (to - from).normalized()}; }

  static Ray FromDirection(const lin::Vec3& origin, const lin::Vec3& dir) { return Ray{origin, dir.normalized()}; }

private:
  Ray(const lin::Vec3& o, const lin::Vec3& d) : origin(o), direction(d) {}
};

#endif // CORE_RAY_HPP
