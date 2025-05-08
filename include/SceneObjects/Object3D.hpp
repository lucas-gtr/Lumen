/**
 * @file Object3D.hpp
 * @brief Header file for the Object3D class.
 */

#ifndef SCENEOBJECTS_OBJECT3D_HPP
#define SCENEOBJECTS_OBJECT3D_HPP

#include <Eigen/Core>
#include <memory>

#include "Core/Transform.hpp"
#include "Geometry/Mesh.hpp"
#include "Surface/Material.hpp"

/**
 * @class Object3D
 * @brief A class representing a 3D object with a mesh and transformation.
 *
 * This class inherits from Transform and encapsulates a Mesh. It provides
 * functionality for setting, getting, and cloning the 3D object.
 */
class Object3D : public Transform {
private:
  Mesh                      m_mesh;
  std::shared_ptr<Material> m_material;

public:
  Object3D(); ///< Default constructor.

  /**
   * @brief Constructs an Object3D with a specified mesh.
   * @param mesh The mesh to associate with the Object3D.
   */
  explicit Object3D(const Mesh& mesh);

  Object3D(const Object3D&)            = default; ///< Default copy constructor.
  Object3D& operator=(const Object3D&) = default; ///< Default copy assignment operator.
  Object3D(Object3D&&)                 = default; ///< Default move constructor.
  Object3D& operator=(Object3D&&)      = default; ///< Default move assignment operator.

  /**
   * @brief Sets the mesh for this 3D object.
   * @param mesh The mesh to set.
   */
  void setMesh(const Mesh& mesh) { this->m_mesh = mesh; }

  /**
   * @brief Sets the material for this 3D object.
   * @param material The material to set.
   */
  void setMaterial(const std::shared_ptr<Material>& material) { this->m_material = material; }

  /**
   * @brief Gets the mesh associated with this 3D object.
   * @return The mesh of the 3D object.
   */
  const Mesh& getMesh() const { return m_mesh; }

  /**
   * @brief Gets a modifiable reference to the mesh of this 3D object.
   * @return A reference to the mesh of the 3D object.
   */
  Mesh& getMesh() { return m_mesh; }

  /**
   * @brief Gets the material associated with this 3D object.
   * @return The material of the 3D object.
   */
  std::shared_ptr<Material> getMaterial() const { return m_material; }

  Eigen::Vector3d getMinBound() const;
  Eigen::Vector3d getMaxBound() const;

  ~Object3D() = default; ///< Default destructor.
};

#endif // SCENEOBJECTS_OBJECT3D_HPP
