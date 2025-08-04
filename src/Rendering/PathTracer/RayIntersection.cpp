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

#include <cassert>

#include "BVH/BVHNode.hpp"
#include "Core/Color.hpp"
#include "Core/ImageTypes.hpp"
#include "Core/Ray.hpp"
#include "Geometry/Mesh.hpp"
#include "Rendering/PathTracer/RayIntersection.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"

namespace RayIntersection {

void processFaceIntersection(const Ray& ray, const Mesh& mesh, const Face& face, RayHitInfo& closest_hit) {
  const Vertex& v0 = mesh.getVertex(face.vertex_indices[0]);
  const Vertex& v1 = mesh.getVertex(face.vertex_indices[1]);
  const Vertex& v2 = mesh.getVertex(face.vertex_indices[2]);

  double        hit_distance = std::numeric_limits<double>::max();
  linalg::Vec3d bary_coords;

  if(!getTriangleIntersection(ray, v0.position, v1.position, v2.position, hit_distance, bary_coords)) {
    return;
  }
  if(hit_distance > 0 && hit_distance < closest_hit.distance) {
    updateHitInfoFromBarycentric(closest_hit, hit_distance, bary_coords, v0, v1, v2);
  }
}

RayHitInfo getMeshIntersectionWithBVH(const Ray& ray, const Mesh& mesh) {
  RayHitInfo hit_info;
  hit_info.distance = std::numeric_limits<double>::max();

  const std::vector<RayBVHHitInfo> bvh_hits = getBVHIntersection(ray, mesh.getBVHRoot());

  for(const auto& bvh_hit : bvh_hits) {
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

  const ColorRGB normal_color     = hit_info.material->getNormal(hit_info.bary_coords);
  linalg::Vec3d  normal_direction = {normal_color.r, normal_color.g, normal_color.b};
  normal_direction                = (normal_direction * 2) - linalg::Vec3d(1.0, 1.0, 1.0);

  hit_info.normal = (tangent_space * normal_direction).normalized();
}

RayHitInfo getObjectIntersection(const Ray& ray, const Object3D* object) {
  const Ray  local_ray = transformRayToObjectSpace(ray, object);
  RayHitInfo hit_info  = getMeshIntersection(local_ray, object->getMesh());

  if(hit_info.distance < std::numeric_limits<double>::max()) {
    transformHitInfoToWorldSpace(hit_info, local_ray, ray, object);
  }

  return hit_info;
}

std::vector<RayBVHHitInfo> getBVHIntersection(const Ray& ray, const BVHNode* bvh_node) {
  std::vector<RayBVHHitInfo>  bvh_hits;
  std::vector<const BVHNode*> node_stack;
  node_stack.push_back(bvh_node);

  const linalg::Vec3d inv_dir = ray.direction.cwiseInverse();

  while(!node_stack.empty()) {
    const BVHNode* node = node_stack.back();
    node_stack.pop_back();

    double hit_distance = std::numeric_limits<double>::max();
    if(!getAABBIntersection(ray.origin, inv_dir, node->getMinBound(), node->getMaxBound(), hit_distance)) {
      continue;
    }

    const int leaf_index = node->getLeafIndex();
    if(leaf_index != -1) {
      bvh_hits.push_back({leaf_index, hit_distance});
    } else {
      BVHNode* left  = node->getLeftChild().get();
      BVHNode* right = node->getRightChild().get();

      double left_child_distance  = std::numeric_limits<double>::max();
      double right_child_distance = std::numeric_limits<double>::max();

      bool left_hit = false;

      if(left != nullptr) {
        left_hit =
            getAABBIntersection(ray.origin, inv_dir, left->getMinBound(), left->getMaxBound(), left_child_distance);
      }
      bool right_hit = false;
      if(right != nullptr) {
        right_hit =
            getAABBIntersection(ray.origin, inv_dir, right->getMinBound(), right->getMaxBound(), right_child_distance);
      }

      if(left_hit && right_hit) {
        if(left_child_distance < right_child_distance) {
          node_stack.push_back(right);
          node_stack.push_back(left);
        } else {
          node_stack.push_back(left);
          node_stack.push_back(right);
        }
      } else if(left_hit) {
        node_stack.push_back(left);
      } else if(right_hit) {
        node_stack.push_back(right);
      }
    }
  }
  return std::move(bvh_hits);
}

RayHitInfo getSceneIntersectionWithBVH(const Ray& ray, const Scene* scene) {
  RayHitInfo                 closest_hit;
  std::vector<RayBVHHitInfo> bvh_hits = getBVHIntersection(ray, scene->getBVHRoot());

  std::sort(bvh_hits.begin(), bvh_hits.end(),
            [](const RayBVHHitInfo& a, const RayBVHHitInfo& b) { return a.distance < b.distance; });

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