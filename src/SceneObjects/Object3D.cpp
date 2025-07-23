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

linalg::Vec3d Object3D::computeBound(
    const std::function<linalg::Vec3d(const linalg::Vec3d&, const linalg::Vec3d&)>& comparator) const {
  const auto& vertices = m_mesh.getVertices();
  if(vertices.empty()) {
    return {0.0, 0.0, 0.0};
  }

  linalg::Vec3d bound = vertices[0].position;
  for(size_t i = 1; i < vertices.size(); ++i) {
    bound = comparator(bound, vertices[i].position);
  }

  return linalg::toVec3(getTransformationMatrix() * linalg::toVec4(bound));
}

linalg::Vec3d Object3D::getMinBound() const {
  return computeBound([](const linalg::Vec3d& a, const linalg::Vec3d& b) { return linalg::cwiseMin(a, b); });
}

linalg::Vec3d Object3D::getMaxBound() const {
  return computeBound([](const linalg::Vec3d& a, const linalg::Vec3d& b) { return linalg::cwiseMax(a, b); });
}

Material* Object3D::getMaterial() const { return m_material; }

Object3D::~Object3D() {
  m_object_deleted_observer.notify(this);
  m_material_changed_observer.clear();
  m_object_deleted_observer.clear();
}