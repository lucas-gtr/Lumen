#include "Core/RayIntersection.hpp"
#include "Core/Mesh.hpp"
#include "Core/Object3D.hpp"
#include "Core/Ray.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>
#include <limits>

double RayIntersection::getTriangleIntersection(const Ray& ray, const Eigen::Vector3d& p0, const Eigen::Vector3d& p1,
                                                const Eigen::Vector3d& p2) {
  const Eigen::Vector3d edge1 = p1 - p0;
  const Eigen::Vector3d edge2 = p2 - p0;
  const Eigen::Vector3d h     = ray.direction.cross(edge2);
  const double          a     = edge1.dot(h);

  if(std::abs(a) < EPSILON) {
    return -1.0;
  }

  const double          f = 1.0 / a;
  const Eigen::Vector3d s = ray.origin - p0;
  const double          u = f * s.dot(h);

  if(u < 0.0 || u > 1.0) {
    return -1.0;
  }

  const Eigen::Vector3d q = s.cross(edge1);
  const double          v = f * ray.direction.dot(q);

  if(v < 0.0 || u + v > 1.0) {
    return -1.0;
  }

  const double hit_distance = f * edge2.dot(q);

  if(hit_distance > EPSILON) {
    return hit_distance;
  }

  return -1.0;
}

RayHitInfo RayIntersection::getMeshIntersection(const Ray& ray, const Mesh& mesh) {
  RayHitInfo hit_info;
  hit_info.distance = std::numeric_limits<double>::max();

  for(const auto& face : mesh.getFaces()) {
    const Vertex& v0 = mesh.getVertex(face.vertexIndices[0]);
    const Vertex& v1 = mesh.getVertex(face.vertexIndices[1]);
    const Vertex& v2 = mesh.getVertex(face.vertexIndices[2]);

    const double hit_distance = getTriangleIntersection(ray, v0.position, v1.position, v2.position);

    if(hit_distance > 0 && hit_distance < hit_info.distance) {
      hit_info.distance = hit_distance;
      hit_info.color    = Eigen::Vector3d(1.0, 1.0, 1.0);
    }
  }
  return hit_info;
}

RayHitInfo RayIntersection::getObjectIntersection(const Ray& ray, const Object3D* object) {
  Eigen::Matrix4d inverse_transform = object->getInverseMatrix();

  Ray transformed_ray;
  transformed_ray.origin    = (inverse_transform * ray.origin.homogeneous()).hnormalized();
  transformed_ray.direction = (inverse_transform.block<3, 3>(0, 0) * ray.direction).normalized();
  RayHitInfo hit_info;
  hit_info = getMeshIntersection(transformed_ray, object->getMesh());

  if(hit_info.distance < std::numeric_limits<double>::max()) {
    const Eigen::Vector3d hit_local = transformed_ray.origin + hit_info.distance * transformed_ray.direction;
    const Eigen::Vector3d hit_world = (object->getTransformationMatrix() * hit_local.homogeneous()).hnormalized();
    hit_info.distance               = (hit_world - ray.origin).norm();
  }

  return hit_info;
}