/**
 * @file RayIntersection.hpp
 * @brief Header file for the RayIntersection class.
 */
#ifndef CORE_RAYINTERSECTION_HPP
#define CORE_RAYINTERSECTION_HPP

#include "Core/Mesh.hpp"
#include "Core/Object3D.hpp"
#include "Core/Ray.hpp"
#include <Eigen/Core>
#include <limits>

/**
 * @struct RayHitInfo
 * @brief Structure that holds information about a ray intersection.
 *
 * This structure contains the distance from the ray origin to the hit point and the color at the intersection.
 */
struct RayHitInfo {
  double          distance = std::numeric_limits<double>::max();
  Eigen::Vector3d color;
};

/**
 * @class RayIntersection
 * @brief Class responsible for ray-triangle and ray-mesh intersection calculations.
 *
 * This class provides methods to calculate the intersection of rays with triangles and meshes.
 * The main functions are `getTriangleIntersection()` for ray-triangle intersections
 * and `getMeshIntersection()` for ray-mesh intersections.
 */
class RayIntersection {
private:
  RayIntersection()  = default;
  ~RayIntersection() = default;

  static constexpr double EPSILON = 1e-6;

public:
  RayIntersection(const RayIntersection&)            = delete;
  RayIntersection& operator=(const RayIntersection&) = delete;
  RayIntersection(RayIntersection&&)                 = delete;
  RayIntersection& operator=(RayIntersection&&)      = delete;
  /**
   * @brief Computes the intersection of a ray with a triangle.
   *
   * This method calculates the intersection of the given ray with a triangle defined by three points.
   *
   * @param ray The ray to check for intersection.
   * @param p0 The first point of the triangle.
   * @param p1 The second point of the triangle.
   * @param p2 The third point of the triangle.
   * @return The distance of the intersection from the ray origin.
   */
  static double getTriangleIntersection(const Ray& ray, const Eigen::Vector3d& p0, const Eigen::Vector3d& p1,
                                        const Eigen::Vector3d& p2);
  /**
   * @brief Computes the intersection of a ray with a mesh.
   *
   * This method checks for intersections between the ray and a mesh. It returns the hit information
   * including the distance and the color at the intersection point.
   *
   * @param ray The ray to check for intersection.
   * @param mesh The mesh to check for intersections.
   * @return A `RayHitInfo` structure containing the intersection distance and color.
   */
  static RayHitInfo getMeshIntersection(const Ray& ray, const Mesh& mesh);

  /**
   * @brief Computes the intersection of a ray with an object.
   *
   * This method checks for intersections between the ray and an object taking
   * into account its transform. It returns the hit information
   * including the distance and the color at the intersection point.
   *
   * @param ray The ray to check for intersection.
   * @param object The object to check for intersections.
   * @return A `RayHitInfo` structure containing the intersection distance and color.
   */
  static RayHitInfo getObjectIntersection(const Ray& ray, const Object3D* object);
};

#endif // CORE_RAYINTERSECTION_HPP