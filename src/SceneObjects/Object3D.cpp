#include <cstddef>
#include <memory>

#include "Core/Math/Vec3.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/Math/lin.hpp"
#include "Geometry/Mesh.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"

Object3D::Object3D() : m_material(std::make_shared<Material>()) {}
Object3D::Object3D(const Mesh& mesh) : m_mesh(mesh), m_material(std::make_shared<Material>()) {}

lin::Vec3 Object3D::getMinBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return lin::Vec3(0.0);
  }

  lin::Vec3 min_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    min_bound = lin::cwiseMin(min_bound, vertices[i].position);
  }

  lin::Vec3 transformed = lin::toVec3(getTransformationMatrix() * lin::toVec4(min_bound));

  return transformed;
}

lin::Vec3 Object3D::getMaxBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return lin::Vec3(0.0);
  }

  lin::Vec3 max_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    max_bound = lin::cwiseMax(max_bound, vertices[i].position);
  }

  lin::Vec3 transformed = lin::toVec3(getTransformationMatrix() * lin::toVec4(max_bound));

  return transformed;
}