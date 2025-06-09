#include <cstddef>
#include <stdexcept>

#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Geometry/Mesh.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"

Object3D::Object3D() : m_material(nullptr) {}
Object3D::Object3D(const Mesh& mesh) : m_mesh(mesh), m_material(nullptr) {}

lin::Vec3d Object3D::getMinBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return {0.0, 0.0, 0.0};
  }

  lin::Vec3d min_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    min_bound = lin::cwiseMin(min_bound, vertices[i].position);
  }

  lin::Vec3d transformed = lin::toVec3(getTransformationMatrix() * lin::toVec4(min_bound));

  return transformed;
}

lin::Vec3d Object3D::getMaxBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return {0.0, 0.0, 0.0};
  }

  lin::Vec3d max_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    max_bound = lin::cwiseMax(max_bound, vertices[i].position);
  }

  lin::Vec3d transformed = lin::toVec3(getTransformationMatrix() * lin::toVec4(max_bound));

  return transformed;
}

const Material& Object3D::getMaterial() const {
  if(m_material == nullptr) {
    throw std::runtime_error("Material is not set for this Object3D.");
  }
  return *m_material;
}