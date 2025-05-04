#include <memory>

#include "Geometry/Mesh.hpp"
#include "Scene/Object3D.hpp"
#include "Surface/Material.hpp"

Object3D::Object3D() : m_material(std::make_shared<Material>()) {}
Object3D::Object3D(const Mesh& mesh) : m_mesh(mesh), m_material(std::make_shared<Material>()) {}