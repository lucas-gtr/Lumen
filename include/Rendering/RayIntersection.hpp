/**
 * @file RayIntersection.hpp
 * @brief Header file for ray intersection functions.
 */
#ifndef RENDERING_RAYINTERSECTION_HPP
#define RENDERING_RAYINTERSECTION_HPP

#include <Eigen/Core>
#include <limits>
#include <vector>

#include "Core/CommonTypes.hpp"
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
  double          distance = std::numeric_limits<double>::max();
  Material*       material = nullptr;
  TextureUV       uvCoordinates;
  Eigen::Vector3d normal;
  Eigen::Vector3d tangent;
  Eigen::Vector3d bitangent;
  Eigen::Vector3d hitPoint;
};

namespace RayIntersection {

struct RayBVHHitInfo {
  int    index_to_check;
  double distance;
};

constexpr double RAY_OFFSET_FACTOR = 0.0001;
constexpr double EPSILON           = 1e-6;

bool getTriangleIntersection(const Ray& ray, const Eigen::Vector3d& p0, const Eigen::Vector3d& p1,
                             const Eigen::Vector3d& p2, double& hit_distance, Eigen::Vector3d& bary_coords);

RayHitInfo getMeshIntersectionWithBVH(const Ray& ray, const Mesh& mesh);
RayHitInfo getMeshIntersectionWithoutBVH(const Ray& ray, const Mesh& mesh);
RayHitInfo getMeshIntersection(const Ray& ray, const Mesh& mesh);

RayHitInfo getObjectIntersection(const Ray& ray, const Object3D* object);

RayHitInfo getSceneIntersectionWithBVH(const Ray& ray, const Scene* scene);
RayHitInfo getSceneIntersectionWithoutBVH(const Ray& ray, const Scene* scene);
RayHitInfo getSceneIntersection(const Ray& ray, const Scene* scene);

bool getAABBIntersection(const Ray& ray, const Eigen::Vector3d& min_bound, const Eigen::Vector3d& max_bound,
                         double& hit_distance);
std::vector<RayBVHHitInfo> getBVHIntersection(const Ray& ray, const BVHNode* bvh_node);

void processFaceIntersection(const Ray& ray, const Mesh& mesh, const Face& face, RayHitInfo& closest_hit);
void updateHitInfoFromBarycentric(RayHitInfo& hit_info, double distance, const Eigen::Vector3d& bary, const Vertex& v0,
                                  const Vertex& v1, const Vertex& v2);
Ray  transformRayToObjectSpace(const Ray& ray, const Object3D* object);
void transformHitInfoToWorldSpace(RayHitInfo& hit_info, const Ray& local_ray, const Ray& original_ray,
                                  const Object3D* object);
void updateNormalWithTangentSpace(RayHitInfo& hit_info);

} // namespace RayIntersection

#endif // RENDERING_RAYINTERSECTION_HPP
