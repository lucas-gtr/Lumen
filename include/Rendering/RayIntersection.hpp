/**
 * @file RayIntersection.hpp
 * @brief Header file for ray intersection functions.
 */
#ifndef RENDERING_RAYINTERSECTION_HPP
#define RENDERING_RAYINTERSECTION_HPP

#include <limits>
#include <vector>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Ray.hpp"
#include "Geometry/Mesh.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include <BVH/BVHNode.hpp>

/**
 * @struct RayHitInfo
 * @brief Structure that holds information about a ray intersection.
 */
struct RayHitInfo {
  double    distance = std::numeric_limits<double>::max();
  Material* material = nullptr;
  TextureUV uvCoordinates;
  lin::Vec3 normal;
  lin::Vec3 tangent;
  lin::Vec3 bitangent;
  lin::Vec3 hitPoint;
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

constexpr double RAY_OFFSET_FACTOR = 0.0001;
constexpr double EPSILON           = 1e-6;

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
bool getTriangleIntersection(const Ray& ray, const lin::Vec3& p0, const lin::Vec3& p1, const lin::Vec3& p2,
                             double& hit_distance, lin::Vec3& bary_coords);

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
 * @param ray The ray to check for intersection.
 * @param min_bound The minimum corner of the AABB.
 * @param max_bound The maximum corner of the AABB.
 * @param hit_distance The distance to the intersection point, if any.
 * @return True if the ray intersects the AABB, false otherwise.
 */
bool getAABBIntersection(const Ray& ray, const lin::Vec3& min_bound, const lin::Vec3& max_bound, double& hit_distance);

/**
 * @brief Gets the intersection information of a ray with a BVH node.
 * @param ray The ray to check for intersection.
 * @param bvh_node The BVH node to check for intersection.
 * @return A vector of RayBVHHitInfo containing the intersection information for each object in the BVH node.
 */
std::vector<RayBVHHitInfo> getBVHIntersection(const Ray& ray, const BVHNode* bvh_node);

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
void updateHitInfoFromBarycentric(RayHitInfo& hit_info, double distance, const lin::Vec3& bary, const Vertex& v0,
                                  const Vertex& v1, const Vertex& v2);

/**
 * @brief Transforms a ray from world space to object space.
 * @param ray The ray to transform.
 * @param object The object to which the ray is being transformed.
 * @return The transformed ray in object space.
 */
Ray transformRayToObjectSpace(const Ray& ray, const Object3D* object);

/**
 * @brief Transforms the hit information from object space to world space.
 * @param hit_info The RayHitInfo to transform.
 * @param local_ray The ray in object space.
 * @param original_ray The original ray in world space.
 * @param object The object to which the hit information belongs.
 */
void transformHitInfoToWorldSpace(RayHitInfo& hit_info, const Ray& local_ray, const Ray& original_ray,
                                  const Object3D* object);

/**
 * @brief Updates the normal vector in the hit information using tangent space.
 * @param hit_info The RayHitInfo to update.
 */
void updateNormalWithTangentSpace(RayHitInfo& hit_info);

} // namespace RayIntersection

#endif // RENDERING_RAYINTERSECTION_HPP
