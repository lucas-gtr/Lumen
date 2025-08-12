/**
 * @file RayIntersection.hpp
 * @brief Header file for ray intersection functions.
 */
#ifndef RENDERING_PATHTRACER_RAYINTERSECTION_HPP
#define RENDERING_PATHTRACER_RAYINTERSECTION_HPP

#include <limits>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>
#include <vector>

#include "Core/ImageTypes.hpp"
#include "Core/Ray.hpp"
#include "Geometry/Mesh.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"

class BVHNode;

/**
 * @struct RayHitInfo
 * @brief Structure that holds information about a ray intersection.
 */
struct RayHitInfo {
  ColorRGB        emitted_light = ColorRGB(0.0);
  linalg::Vec3d   normal;
  linalg::Vec3d   tangent;
  linalg::Vec3d   bitangent;
  linalg::Vec3d   position;
  TextureUV       bary_coords;
  double          distance = std::numeric_limits<double>::max();
  double          area     = 0.0;
  const Material* material = nullptr;
};

/**
 * @namespace RayIntersection
 * @brief Namespace for ray intersection functions.
 */
namespace RayIntersection {
/**
 * @struct RayBVHHitInfo
 * @brief Structure that holds information about a ray intersection with a BVH node.
 *
 * This structure is used to store the index of the object to check and the distance of the intersection.
 */
struct RayBVHHitInfo {
  int    index_to_check;
  double distance;
};

constexpr double RAY_OFFSET_FACTOR      = 1e-9;
constexpr double INTERSECTION_TOLERANCE = 1e-6;

/**
 * @brief Checks for intersection between a ray and a triangle defined by three points.
 * @param ray The ray to check for intersection.
 * @param p0 The first vertex of the triangle.
 * @param p1 The second vertex of the triangle.
 * @param p2 The third vertex of the triangle.
 * @param hit_distance The distance to the intersection point, if any.
 * @param bary_coords The barycentric coordinates of the intersection point, if any.
 * @return True if the ray intersects the triangle, false otherwise.
 */
inline bool getTriangleIntersection(const Ray& ray, const linalg::Vec3d& p0, const linalg::Vec3d& p1,
                                    const linalg::Vec3d& p2, double& hit_distance, linalg::Vec3d& bary_coords) {
  const linalg::Vec3d edge1 = p1 - p0;
  const linalg::Vec3d edge2 = p2 - p0;
  const linalg::Vec3d h     = ray.direction.cross(edge2);
  const double        a     = linalg::dot(edge1, h);

  if(a > -INTERSECTION_TOLERANCE && a < INTERSECTION_TOLERANCE) {
    return false;
  }

  const double        f = 1.0 / a;
  const linalg::Vec3d s = ray.origin - p0;
  const double        u = f * linalg::dot(s, h);

  if(u < -INTERSECTION_TOLERANCE || u > 1.0 + INTERSECTION_TOLERANCE) {
    return false;
  }

  const linalg::Vec3d q = s.cross(edge1);
  const double        v = f * linalg::dot(ray.direction, q);

  if(v < -INTERSECTION_TOLERANCE || u + v > 1.0 + INTERSECTION_TOLERANCE) {
    return false;
  }

  hit_distance = f * linalg::dot(edge2, q);

  if(hit_distance > INTERSECTION_TOLERANCE) {
    bary_coords = {1.0 - u - v, u, v};
    return true;
  }

  return false;
}

/**
 * @brief Processes the intersection of a ray with a mesh using a BVH.
 * @param ray The ray to check for intersection.
 * @param mesh The mesh containing the face.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getMeshIntersectionWithBVH(const Ray& ray, const Mesh& mesh);

/**
 * @brief Processes the intersection of a ray with a mesh without using a BVH.
 * @param ray The ray to check for intersection.
 * @param mesh The mesh containing the face.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getMeshIntersectionWithoutBVH(const Ray& ray, const Mesh& mesh);

/**
 * @brief Processes the intersection of a ray with a mesh.
 * @param ray The ray to check for intersection.
 * @param mesh The mesh containing the face.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getMeshIntersection(const Ray& ray, const Mesh& mesh);

/**
 * @brief Gets the intersection information of a ray with an object in the scene.
 * @param ray The ray to check for intersection.
 * @param object The object to check for intersection.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getObjectIntersection(const Ray& ray, const Object3D* object);

/**
 * @brief Gets the intersection information of a ray with the scene, using BVH.
 * @param ray The ray to check for intersection.
 * @param scene The scene containing the objects.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getSceneIntersectionWithBVH(const Ray& ray, const Scene* scene);

/**
 * @brief Gets the intersection information of a ray with the scene, without using BVH.
 * @param ray The ray to check for intersection.
 * @param scene The scene containing the objects.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getSceneIntersectionWithoutBVH(const Ray& ray, const Scene* scene);

/**
 * @brief Gets the intersection information of a ray with the scene.
 * @param ray The ray to check for intersection.
 * @param scene The scene containing the objects.
 * @return RayHitInfo containing the intersection information if an intersection occurs, otherwise an empty RayHitInfo.
 */
RayHitInfo getSceneIntersection(const Ray& ray, const Scene* scene);

/**
 * @brief Checks if a ray intersects with an axis-aligned bounding box (AABB).
 * @param origin The origin of the ray.
 * @param inv_dir The inverse direction of the ray.
 * @param min_bound The minimum corner of the AABB.
 * @param max_bound The maximum corner of the AABB.
 * @param hit_distance The distance to the intersection point, if any.
 * @return True if the ray intersects the AABB, false otherwise.
 */
inline bool getAABBIntersection(const linalg::Vec3d& origin, const linalg::Vec3d& inv_dir,
                                const linalg::Vec3d& min_bound, const linalg::Vec3d& max_bound, double& hit_distance) {
  const linalg::Vec3d t0 = linalg::cwiseProduct((min_bound - origin), inv_dir);
  const linalg::Vec3d t1 = linalg::cwiseProduct((max_bound - origin), inv_dir);

  const linalg::Vec3d t_min = linalg::cwiseMin(t0, t1);
  const linalg::Vec3d t_max = linalg::cwiseMax(t0, t1);

  const double t_entry = t_min.maxValue();
  const double t_exit  = t_max.minValue();

  if(t_entry > t_exit || t_exit < 0) {
    return false;
  }
  hit_distance = t_entry;
  return true;
}

/**
 * @brief Gets the intersection information of a ray with a BVH node.
 * @param ray The ray to check for intersection.
 * @param bvh_node The BVH node to check for intersection.
 * @return A vector of RayBVHHitInfo containing the intersection information for each object in the BVH node.
 */
std::vector<RayBVHHitInfo> getBVHIntersection(const Ray& ray, const BVHNode* bvh_node);

/**
 * @brief Gets the name of the object that a ray intersects with in the scene.
 * @param ray The ray to check for intersection.
 * @param scene The scene containing the objects.
 * @return The name of the object that the ray intersects with, or an empty string if no intersection occurs.
 */
std::string getObjectNameFromHit(const Ray& ray, const Scene* scene);

/**
 * @brief Processes the intersection of a ray with a face in a mesh.
 * @param ray The ray to check for intersection.
 * @param mesh The mesh containing the face.
 * @param face The face to check for intersection.
 * @param closest_hit The RayHitInfo to update with the intersection information if an intersection occurs.
 */
void processFaceIntersection(const Ray& ray, const Mesh& mesh, const Face& face, RayHitInfo& closest_hit);

/**
 * @brief Updates the RayHitInfo with barycentric coordinates and vertex information.
 * @param hit_info The RayHitInfo to update.
 * @param distance The distance to the intersection point.
 * @param bary The barycentric coordinates of the intersection point.
 * @param v0 The first vertex of the triangle.
 * @param v1 The second vertex of the triangle.
 * @param v2 The third vertex of the triangle.
 */
inline void updateHitInfoFromBarycentric(RayHitInfo& hit_info, double distance, const linalg::Vec3d& bary,
                                         const Vertex& v0, const Vertex& v1, const Vertex& v2) {
  hit_info.distance      = distance;
  hit_info.bary_coords.u = bary.x * v0.uv_coord.u + bary.y * v1.uv_coord.u + bary.z * v2.uv_coord.u;
  hit_info.bary_coords.v = bary.x * v0.uv_coord.v + bary.y * v1.uv_coord.v + bary.z * v2.uv_coord.v;

  hit_info.normal    = (bary.x * v0.normal + bary.y * v1.normal + bary.z * v2.normal).normalized();
  hit_info.tangent   = (bary.x * v0.tangent + bary.y * v1.tangent + bary.z * v2.tangent).normalized();
  hit_info.bitangent = (bary.x * v0.bitangent + bary.y * v1.bitangent + bary.z * v2.bitangent).normalized();
  hit_info.area      = (v1.position - v0.position).cross(v2.position - v0.position).length() * HALF;
}
/**
 * @brief Transforms a ray from world space to object space.
 * @param ray The ray to transform.
 * @param object The object to which the ray is being transformed.
 * @return The transformed ray in object space.
 */
inline Ray transformRayToObjectSpace(const Ray& ray, const Object3D* object) {
  const linalg::Mat4d inv_matrix = object->getInverseMatrix();

  const linalg::Vec3d origin    = linalg::toVec3((inv_matrix * linalg::toVec4(ray.origin)));
  const linalg::Vec3d direction = (inv_matrix.topLeft3x3() * ray.direction).normalized();

  return Ray::FromDirection(origin, direction);
}

/**
 * @brief Transforms the hit information from object space to world space.
 * @param hit_info The RayHitInfo to transform.
 * @param local_ray The ray in object space.
 * @param original_ray The original ray in world space.
 * @param object The object to which the hit information belongs.
 */
inline void transformHitInfoToWorldSpace(RayHitInfo& hit_info, const Ray& local_ray, const Ray& original_ray,
                                         const Object3D* object) {
  const linalg::Vec3d local_position = local_ray.origin + hit_info.distance * local_ray.direction;
  const linalg::Vec3d world_position =
      linalg::toVec3((object->getTransformationMatrix() * linalg::toVec4(local_position)));

  hit_info.distance = (world_position - original_ray.origin).length();
  hit_info.position = world_position;
  hit_info.material = object->getMaterial();

  hit_info.emitted_light =
      hit_info.material->getEmissive(hit_info.bary_coords) * hit_info.material->getEmissiveIntensity();

  const linalg::Mat3d rotation_scale_matrix = object->getTransformationMatrix().topLeft3x3();
  const linalg::Vec3d tangent_world         = rotation_scale_matrix * hit_info.tangent;
  const linalg::Vec3d bitangent_world       = rotation_scale_matrix * hit_info.bitangent;
  const double        area_scale            = tangent_world.cross(bitangent_world).length();
  hit_info.area *= area_scale;

  const linalg::Mat3d normal_matrix = object->getNormalMatrix();
  hit_info.normal                   = (normal_matrix * hit_info.normal).normalized();
  hit_info.tangent                  = (normal_matrix * hit_info.tangent).normalized();
  hit_info.bitangent                = (normal_matrix * hit_info.bitangent).normalized();
}

/**
 * @brief Updates the normal vector in the hit information using tangent space.
 * @param hit_info The RayHitInfo to update.
 */
void updateNormalWithTangentSpace(RayHitInfo& hit_info);

} // namespace RayIntersection

#endif // RENDERING_PATHTRACER_RAYINTERSECTION_HPP
