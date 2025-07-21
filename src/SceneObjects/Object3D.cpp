#include <cstddef>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>
#include <stdexcept>

#include "Geometry/Mesh.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include "Surface/MaterialManager.hpp"

Object3D::Object3D() : m_material(MaterialManager::DefaultMaterial()) {}

Object3D::Object3D(const Mesh& mesh) : m_mesh(mesh), m_material(MaterialManager::DefaultMaterial()) {}

void Object3D::setMaterial(Material* material) {
  if(material == nullptr) {
    material = MaterialManager::DefaultMaterial();
  }
  m_material = material;
  m_material_changed_observer.notify(this);
}

linalg::Vec3d Object3D::getMinBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return {0.0, 0.0, 0.0};
  }

  linalg::Vec3d min_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    min_bound = linalg::cwiseMin(min_bound, vertices[i].position);
  }

  linalg::Vec3d transformed = linalg::toVec3(getTransformationMatrix() * linalg::toVec4(min_bound));

  return transformed;
}

linalg::Vec3d Object3D::getMaxBound() const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return {0.0, 0.0, 0.0};
  }

  linalg::Vec3d max_bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    max_bound = linalg::cwiseMax(max_bound, vertices[i].position);
  }

  linalg::Vec3d transformed = linalg::toVec3(getTransformationMatrix() * linalg::toVec4(max_bound));

  return transformed;
}

Material* Object3D::getMaterial() const {
  if(m_material == nullptr) {
    throw std::runtime_error("Material is not set for this Object3D.");
  }
  return m_material;
}

Object3D::~Object3D() {
  m_object_deleted_observer.notify(this);
  m_material_changed_observer.clear();
  m_object_deleted_observer.clear();
}