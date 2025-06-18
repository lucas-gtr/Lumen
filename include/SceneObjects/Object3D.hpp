/**
 * @file Object3D.hpp
 * @brief Header file for the Object3D class.
 */

#ifndef SCENEOBJECTS_OBJECT3D_HPP
#define SCENEOBJECTS_OBJECT3D_HPP

#include "Core/Math/Vec3.hpp"
#include "Core/Observer.hpp"
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
  Mesh      m_mesh;
  Material* m_material;

  Observer<const Object3D*> m_material_changed_observer;

public:
  Object3D(); ///< Default constructor.

  /**
   * @brief Constructs an Object3D with a specified mesh.
   * @param mesh The mesh to associate with the Object3D.
   */
  explicit Object3D(const Mesh& mesh);

  Object3D(const Object3D&)            = delete;
  Object3D& operator=(const Object3D&) = delete;
  Object3D(Object3D&&)                 = delete;
  Object3D& operator=(Object3D&&)      = delete;

  /**
   * @brief Gets the observer that notifies when the material changes.
   * @return A reference to the observer that notifies about material changes.
   */
  Observer<const Object3D*>& getMaterialChangedObserver() { return m_material_changed_observer; }

  /**
   * @brief Sets the mesh for this 3D object.
   * @param mesh The mesh to set.
   */
  void setMesh(const Mesh& mesh) { this->m_mesh = mesh; }

  /**
   * @brief Sets the material for this 3D object.
   * @param material The material to set.
   */
  void setMaterial(Material* material);

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
  Material* getMaterial() const;

  /**
   * @brief Gets the minimum bounding box corner of the object.
   * @return The minimum bounding box corner as a Vec3d.
   */
  lin::Vec3d getMinBound() const;

  /**
   * @brief Gets the maximum bounding box corner of the object.
   * @return The maximum bounding box corner as a Vec3d.
   */
  lin::Vec3d getMaxBound() const;

  ~Object3D() = default; ///< Default destructor.
};

#endif // SCENEOBJECTS_OBJECT3D_HPP
