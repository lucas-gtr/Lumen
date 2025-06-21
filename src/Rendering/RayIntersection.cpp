#include <algorithm>
#include <cmath>
#include <limits>
#include <stack>
#include <string>
#include <vector>

#include "BVH/BVHNode.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Core/Ray.hpp"
#include "Geometry/Mesh.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"

namespace RayIntersection {

bool getTriangleIntersection(const Ray& ray, const lin::Vec3d& p0, const lin::Vec3d& p1, const lin::Vec3d& p2,
                             double& hit_distance, lin::Vec3d& bary_coords) {
  const lin::Vec3d edge1 = p1 - p0;
  const lin::Vec3d edge2 = p2 - p0;
  const lin::Vec3d h     = ray.direction.cross(edge2);
  const double     a     = lin::dot(edge1, h);

  if(std::abs(a) < EPSILON) {
    return false;
  }

  const double     f = 1.0 / a;
  const lin::Vec3d s = ray.origin - p0;
  const double     u = f * lin::dot(s, h);

  if(u < 0.0 || u > 1.0) {
    return false;
  }

  const lin::Vec3d q = s.cross(edge1);
  const double     v = f * lin::dot(ray.direction, q);

  if(v < 0.0 || u + v > 1.0) {
    return false;
  }

  hit_distance = f * lin::dot(edge2, q);

  if(hit_distance > EPSILON) {
    bary_coords = {1.0 - u - v, u, v};
    return true;
  }

  return false;
}

void processFaceIntersection(const Ray& ray, const Mesh& mesh, const Face& face, RayHitInfo& closest_hit) {
  const Vertex& v0 = mesh.getVertex(face.vertexIndices[0]);
  const Vertex& v1 = mesh.getVertex(face.vertexIndices[1]);
  const Vertex& v2 = mesh.getVertex(face.vertexIndices[2]);

  double     hit_distance = std::numeric_limits<double>::max();
  lin::Vec3d bary_coords;

  if(!getTriangleIntersection(ray, v0.position, v1.position, v2.position, hit_distance, bary_coords)) {
    return;
  }

  if(hit_distance > 0 && hit_distance < closest_hit.distance) {
    updateHitInfoFromBarycentric(closest_hit, hit_distance, bary_coords, v0, v1, v2);
  }
}

void updateHitInfoFromBarycentric(RayHitInfo& hit_info, double distance, const lin::Vec3d& bary, const Vertex& v0,
                                  const Vertex& v1, const Vertex& v2) {
  hit_info.distance        = distance;
  hit_info.uvCoordinates.u = bary.x * v0.uv_coord.u + bary.y * v1.uv_coord.u + bary.z * v2.uv_coord.u;
  hit_info.uvCoordinates.v = bary.x * v0.uv_coord.v + bary.y * v1.uv_coord.v + bary.z * v2.uv_coord.v;
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

  const lin::Mat3d tangent_space = lin::Mat3d::FromColumns(hit_info.tangent, hit_info.bitangent, hit_info.normal);

  const ColorRGB normal_color     = hit_info.material->getNormal(hit_info.uvCoordinates);
  lin::Vec3d     normal_direction = {normal_color.r, normal_color.g, normal_color.b};
  normal_direction                = (normal_direction * 2) - lin::Vec3d(1.0, 1.0, 1.0);

  hit_info.normal = (tangent_space * normal_direction).normalized();
}

Ray transformRayToObjectSpace(const Ray& ray, const Object3D* object) {
  const lin::Mat4d inv_matrix = object->getInverseMatrix();

  const lin::Vec3d origin    = lin::toVec3((inv_matrix * lin::toVec4(ray.origin)));
  const lin::Vec3d direction = (inv_matrix.topLeft3x3() * ray.direction).normalized();

  Ray local_ray = Ray::FromDirection(origin, direction);

  return local_ray;
}

void transformHitInfoToWorldSpace(RayHitInfo& hit_info, const Ray& local_ray, const Ray& original_ray,
                                  const Object3D* object) {
  const lin::Vec3d hit_local = local_ray.origin + hit_info.distance * local_ray.direction;
  const lin::Vec3d hit_world = lin::toVec3((object->getTransformationMatrix() * lin::toVec4(hit_local)));

  hit_info.distance = (hit_world - original_ray.origin).length();
  hit_info.hitPoint = hit_world;
  hit_info.material = object->getMaterial();

  const lin::Mat3d normal_matrix = object->getNormalMatrix();
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

bool getAABBIntersection(const Ray& ray, const lin::Vec3d& min_bound, const lin::Vec3d& max_bound,
                         double& hit_distance) {
  const lin::Vec3d inv_dir = ray.direction.cwiseInverse();
  const lin::Vec3d t0      = lin::cwiseProduct((min_bound - ray.origin), inv_dir);
  const lin::Vec3d t1      = lin::cwiseProduct((max_bound - ray.origin), inv_dir);

  const lin::Vec3d t_min = lin::cwiseMin(t0, t1);
  const lin::Vec3d t_max = lin::cwiseMax(t0, t1);

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
  std::stack<const BVHNode*> nodeStack;
  nodeStack.push(bvh_node);

  while(!nodeStack.empty()) {
    const BVHNode* node = nodeStack.top();
    nodeStack.pop();

    double hit_distance = std::numeric_limits<double>::max();
    if(!getAABBIntersection(ray, node->getMinBound(), node->getMaxBound(), hit_distance)) {
      continue;
    }

    const int leafIndex = node->getLeafIndex();
    if(leafIndex != -1) {
      bvh_hits.push_back({leafIndex, hit_distance});
    } else {
      BVHNode* left  = node->getLeftChild().get();
      BVHNode* right = node->getRightChild().get();

      double left_child_distance  = std::numeric_limits<float>::max();
      double right_child_distance = std::numeric_limits<float>::max();

      bool leftHit = false;

      if(left != nullptr) {
        leftHit = getAABBIntersection(ray, left->getMinBound(), left->getMaxBound(), left_child_distance);
      }
      bool rightHit = false;
      if(right != nullptr) {
        rightHit = getAABBIntersection(ray, right->getMinBound(), right->getMaxBound(), right_child_distance);
      }

      if(leftHit && rightHit) {
        if(left_child_distance < right_child_distance) {
          nodeStack.push(right);
          nodeStack.push(left);
        } else {
          nodeStack.push(left);
          nodeStack.push(right);
        }
      } else if(leftHit) {
        nodeStack.push(left);
      } else if(rightHit) {
        nodeStack.push(right);
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