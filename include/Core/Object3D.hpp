/**
 * @file Object3D.hpp
 * @brief Header file for the Object3D class.
 */

#ifndef CORE_OBJECT3D_HPP
#define CORE_OBJECT3D_HPP

#include "Mesh.hpp"
#include "Transform.hpp"

/**
 * @class Object3D
 * @brief A class representing a 3D object with a mesh and transformation.
 *
 * This class inherits from Transform and encapsulates a Mesh. It provides
 * functionality for setting, getting, and cloning the 3D object.
 */
class Object3D : public Transform {
private:
  Mesh m_mesh;

public:
  Object3D() = default; ///< Default constructor.

  /**
   * @brief Constructs an Object3D with a specified mesh.
   * @param mesh The mesh to associate with the Object3D.
   */
  explicit Object3D(const Mesh& mesh) : m_mesh(mesh) {}

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
   * @brief Gets the mesh associated with this 3D object.
   * @return The mesh of the 3D object.
   */
  Mesh getMesh() const { return m_mesh; }

  ~Object3D() = default; ///< Default destructor.
};

#endif // CORE_OBJECT3D_HPP
