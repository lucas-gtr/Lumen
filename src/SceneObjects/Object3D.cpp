#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cstddef>
#include <memory>

#include "Geometry/Mesh.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"

Object3D::Object3D() : m_material(std::make_shared<Material>()) {}
Object3D::Object3D(const Mesh& mesh) : m_mesh(mesh), m_material(std::make_shared<Material>()) {}

Eigen::Vector3d Object3D::getMinBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return Eigen::Vector3d::Zero();
  }

  Eigen::Vector3d min_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    min_bound = min_bound.cwiseMin(vertices[i].position);
  }

  Eigen::Vector4d transformed = getTransformationMatrix() * min_bound.homogeneous();

  return transformed.head<3>();
}

Eigen::Vector3d Object3D::getMaxBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return Eigen::Vector3d::Zero();
  }

  Eigen::Vector3d max_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    max_bound = max_bound.cwiseMax(vertices[i].position);
  }

  Eigen::Vector4d transformed = getTransformationMatrix() * max_bound.homogeneous();

  return transformed.head<3>();
}