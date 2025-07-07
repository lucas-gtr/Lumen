#include <algorithm>
#include <cmath>
#include <limits>
#include <linalg/Mat3.hpp>
#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>
#include <stack>
#include <string>
#include <vector>

#include "BVH/BVHNode.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Ray.hpp"
#include "Geometry/Mesh.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"

namespace RayIntersection {

bool getTriangleIntersection(const Ray& ray, const linalg::Vec3d& p0, const linalg::Vec3d& p1, const linalg::Vec3d& p2,
                             double& hit_distance, linalg::Vec3d& bary_coords) {
  const linalg::Vec3d edge1 = p1 - p0;
  const linalg::Vec3d edge2 = p2 - p0;
  const linalg::Vec3d h     = ray.direction.cross(edge2);
  const double     a     = linalg::dot(edge1, h);

  if(std::abs(a) < EPSILON) {
    return false;
  }

  const double     f = 1.0 / a;
  const linalg::Vec3d s = ray.origin - p0;
  const double     u = f * linalg::dot(s, h);

  if(u < 0.0 || u > 1.0) {
    return false;
  }

  const linalg::Vec3d q = s.cross(edge1);
  const double     v = f * linalg::dot(ray.direction, q);

  if(v < 0.0 || u + v > 1.0) {
    return false;
  }

  hit_distance = f * linalg::dot(edge2, q);

  if(hit_distance > EPSILON) {
    bary_coords = {1.0 - u - v, u, v};
    return true;
  }

  return false;
}

void processFaceIntersection(const Ray& ray, const Mesh& mesh, const Face& face, RayHitInfo& closest_hit) {
  const Vertex& v0 = mesh.getVertex(face.vertex_indices[0]);
  const Vertex& v1 = mesh.getVertex(face.vertex_indices[1]);
  const Vertex& v2 = mesh.getVertex(face.vertex_indices[2]);

  double     hit_distance = std::numeric_limits<double>::max();
  linalg::Vec3d bary_coords;

  if(!getTriangleIntersection(ray, v0.position, v1.position, v2.position, hit_distance, bary_coords)) {
    return;
  }

  if(hit_distance > 0 && hit_distance < closest_hit.distance) {
    updateHitInfoFromBarycentric(closest_hit, hit_distance, bary_coords, v0, v1, v2);
  }
}

void updateHitInfoFromBarycentric(RayHitInfo& hit_info, double distance, const linalg::Vec3d& bary, const Vertex& v0,
                                  const Vertex& v1, const Vertex& v2) {
  hit_info.distance        = distance;
  hit_info.uv_coordinates.u = bary.x * v0.uv_coord.u + bary.y * v1.uv_coord.u + bary.z * v2.uv_coord.u;
  hit_info.uv_coordinates.v = bary.x * v0.uv_coord.v + bary.y * v1.uv_coord.v + bary.z * v2.uv_coord.v;
  hit_info.normal          = (bary.x * v0.normal + bary.y * v1.normal + bary.z * v2.normal).normalized();
  hit_info.tangent         = (bary.x * v0.tangent + bary.y * v1.tangent + bary.z * v2.tangent).normalized();
  hit_info.bitangent       = (bary.x * v0.bitangent + bary.y * v1.bitangent + bary.z * v2.bitangent).normalized();
}

RayHitInfo getMeshIntersectionWithBVH(const Ray& ray, const Mesh& mesh) {
  RayHitInfo hit_info;
  hit_info.distance = std::numeric_limits<double>::max();

  const std::vector<RayBVHHitInfo> bvh_hits = getBVHIntersection(ray, mesh.getBVHRoot());

  for(const auto& bvh_hit : bvh_hits) {
    if(bvh_hit.distance > hit_info.distance) {
      return hit_info;
    }
    const Face& face = mesh.getFaces()[bvh_hit.index_to_check];
    processFaceIntersection(ray, mesh, face, hit_info);
  }

  return hit_info;
}

RayHitInfo getMeshIntersectionWithoutBVH(const Ray& ray, const Mesh& mesh) {
  RayHitInfo hit_info;
  hit_info.distance = std::numeric_limits<double>::max();

  for(const auto& face : mesh.getFaces()) {
    processFaceIntersection(ray, mesh, face, hit_info);
  }

  return hit_info;
}

RayHitInfo getMeshIntersection(const Ray& ray, const Mesh& mesh) {
  if(mesh.getBVHRoot() != nullptr) {
    return getMeshIntersectionWithBVH(ray, mesh);
  }
  return getMeshIntersectionWithoutBVH(ray, mesh);
}

void updateNormalWithTangentSpace(RayHitInfo& hit_info) {
  if(hit_info.material == nullptr) {
    return;
  }

  const linalg::Mat3d tangent_space = linalg::Mat3d::FromColumns(hit_info.tangent, hit_info.bitangent, hit_info.normal);

  const ColorRGB normal_color     = hit_info.material->getNormal(hit_info.uv_coordinates);
  linalg::Vec3d     normal_direction = {normal_color.r, normal_color.g, normal_color.b};
  normal_direction                = (normal_direction * 2) - linalg::Vec3d(1.0, 1.0, 1.0);

  hit_info.normal = (tangent_space * normal_direction).normalized();
}

Ray transformRayToObjectSpace(const Ray& ray, const Object3D* object) {
  const linalg::Mat4d inv_matrix = object->getInverseMatrix();

  const linalg::Vec3d origin    = linalg::toVec3((inv_matrix * linalg::toVec4(ray.origin)));
  const linalg::Vec3d direction = (inv_matrix.topLeft3x3() * ray.direction).normalized();

  Ray local_ray = Ray::FromDirection(origin, direction);

  return local_ray;
}

void transformHitInfoToWorldSpace(RayHitInfo& hit_info, const Ray& local_ray, const Ray& original_ray,
                                  const Object3D* object) {
  const linalg::Vec3d hit_local = local_ray.origin + hit_info.distance * local_ray.direction;
  const linalg::Vec3d hit_world = linalg::toVec3((object->getTransformationMatrix() * linalg::toVec4(hit_local)));

  hit_info.distance = (hit_world - original_ray.origin).length();
  hit_info.hit_point = hit_world;
  hit_info.material = object->getMaterial();

  const linalg::Mat3d normal_matrix = object->getNormalMatrix();
  hit_info.normal                = (normal_matrix * hit_info.normal).normalized();
  hit_info.tangent               = (normal_matrix * hit_info.tangent).normalized();
  hit_info.bitangent             = (normal_matrix * hit_info.bitangent).normalized();
}

RayHitInfo getObjectIntersection(const Ray& ray, const Object3D* object) {
  const Ray  local_ray = transformRayToObjectSpace(ray, object);
  RayHitInfo hit_info  = getMeshIntersection(local_ray, object->getMesh());

  if(hit_info.distance < std::numeric_limits<double>::max()) {
    transformHitInfoToWorldSpace(hit_info, local_ray, ray, object);
  }

  return hit_info;
}

bool getAABBIntersection(const Ray& ray, const linalg::Vec3d& min_bound, const linalg::Vec3d& max_bound,
                         double& hit_distance) {
  const linalg::Vec3d inv_dir = ray.direction.cwiseInverse();
  const linalg::Vec3d t0      = linalg::cwiseProduct((min_bound - ray.origin), inv_dir);
  const linalg::Vec3d t1      = linalg::cwiseProduct((max_bound - ray.origin), inv_dir);

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

std::vector<RayBVHHitInfo> getBVHIntersection(const Ray& ray, const BVHNode* bvh_node) {
  std::vector<RayBVHHitInfo> bvh_hits;
  std::stack<const BVHNode*> node_stack;
  node_stack.push(bvh_node);

  while(!node_stack.empty()) {
    const BVHNode* node = node_stack.top();
    node_stack.pop();

    double hit_distance = std::numeric_limits<double>::max();
    if(!getAABBIntersection(ray, node->getMinBound(), node->getMaxBound(), hit_distance)) {
      continue;
    }

    const int leaf_index = node->getLeafIndex();
    if(leaf_index != -1) {
      bvh_hits.push_back({leaf_index, hit_distance});
    } else {
      BVHNode* left  = node->getLeftChild().get();
      BVHNode* right = node->getRightChild().get();

      double left_child_distance  = std::numeric_limits<float>::max();
      double right_child_distance = std::numeric_limits<float>::max();

      bool left_hit = false;

      if(left != nullptr) {
        left_hit = getAABBIntersection(ray, left->getMinBound(), left->getMaxBound(), left_child_distance);
      }
      bool right_hit = false;
      if(right != nullptr) {
        right_hit = getAABBIntersection(ray, right->getMinBound(), right->getMaxBound(), right_child_distance);
      }

      if(left_hit && right_hit) {
        if(left_child_distance < right_child_distance) {
          node_stack.push(right);
          node_stack.push(left);
        } else {
          node_stack.push(left);
          node_stack.push(right);
        }
      } else if(left_hit) {
        node_stack.push(left);
      } else if(right_hit) {
        node_stack.push(right);
      }
    }
  }

  std::sort(bvh_hits.begin(), bvh_hits.end(),
            [](const RayBVHHitInfo& a, const RayBVHHitInfo& b) { return a.distance < b.distance; });
  return bvh_hits;
}

RayHitInfo getSceneIntersectionWithBVH(const Ray& ray, const Scene* scene) {
  RayHitInfo                       closest_hit;
  const std::vector<RayBVHHitInfo> bvh_hits = getBVHIntersection(ray, scene->getBVHRoot());

  for(const auto& bvh_hit : bvh_hits) {
    if(bvh_hit.distance > closest_hit.distance) {
      updateNormalWithTangentSpace(closest_hit);
      return closest_hit;
    }
    const Object3D*  object   = scene->getObjectList()[bvh_hit.index_to_check];
    const RayHitInfo hit_info = getObjectIntersection(ray, object);
    if(hit_info.distance < closest_hit.distance) {
      closest_hit = hit_info;
    }
  }

  updateNormalWithTangentSpace(closest_hit);
  return closest_hit;
}

RayHitInfo getSceneIntersectionWithoutBVH(const Ray& ray, const Scene* scene) {
  RayHitInfo closest_hit;
  closest_hit.distance = std::numeric_limits<double>::max();

  for(const auto& object : scene->getObjectList()) {
    const RayHitInfo hit_info = getObjectIntersection(ray, object);
    if(hit_info.distance < closest_hit.distance) {
      closest_hit = hit_info;
    }
  }

  updateNormalWithTangentSpace(closest_hit);
  return closest_hit;
}

std::string getObjectNameFromHit(const Ray& ray, const Scene* scene) {
  RayHitInfo closest_hit;
  closest_hit.distance = std::numeric_limits<double>::max();

  Object3D* closest_object = nullptr;
  for(const auto& object : scene->getObjectList()) {
    const RayHitInfo hit_info = getObjectIntersection(ray, object);
    if(hit_info.distance < closest_hit.distance) {
      closest_hit    = hit_info;
      closest_object = object;
    }
  }

  return scene->getObjectName(closest_object);
}

RayHitInfo getSceneIntersection(const Ray& ray, const Scene* scene) {
  if(scene->getBVHRoot() != nullptr) {
    return getSceneIntersectionWithBVH(ray, scene);
  }
  return getSceneIntersectionWithoutBVH(ray, scene);
}
} // namespace RayIntersection