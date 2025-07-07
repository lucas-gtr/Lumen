/**
 * @file Transform.hpp
 * @brief Header file for the Transform class.
 */
#ifndef CORE_TRANSFORM_HPP
#define CORE_TRANSFORM_HPP

#include <linalg/Mat3.hpp>
#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>

#include "Core/MathConstants.hpp"
#include "Core/Observer.hpp"

/**
 * @class Transform
 * @brief Represents a 3D transformation consisting of position, rotation, and scale.
 *
 * This class handles the transformation operations in 3D space, including setting and getting the position,
 * rotation, and scale of an object. It also computes the corresponding transformation matrices, which can
 * be used for rendering or physics simulations.
 */
class Transform {
private:
  linalg::Mat4d m_transformation_matrix = linalg::Mat4d::Identity();
  linalg::Mat4d m_inverse_matrix        = linalg::Mat4d::Identity();

  linalg::Mat4d m_translation_matrix = linalg::Mat4d::Identity();
  linalg::Mat3d m_rotation_matrix    = linalg::Mat3d::Identity();
  linalg::Mat3d m_scale_matrix       = linalg::Mat3d::Identity();

  linalg::Mat3d m_normal_matrix = linalg::Mat3d::Identity();

  linalg::Vec3d m_position;
  linalg::Vec3d m_rotation;
  linalg::Vec3d m_scale;

  Observer<> m_transformation_changed_observer;

  void updateTranslationMatrix();
  void updateScaleMatrix();
  void updateTransformationMatrix();
  void updateInverseMatrix();
  void updateNormalMatrix();
  void updateRotationMatrix();

public:
  /**
   * @brief Default constructor that initializes position to (0, 0, 0), rotation to (0, 0, 0),
   * and scale to (1, 1, 1).
   */
  Transform() : m_position(linalg::Vec3d(0.0)), m_rotation(linalg::Vec3d(0.0)), m_scale(linalg::Vec3d(1.0)) {}

  /**
   * @brief Constructs a Transform with specific position, rotation, and scale.
   * @param position The position of the object.
   * @param rotation The rotation of the object in Euler angles (degrees).
   * @param scale The scale of the object.
   */
  Transform(const linalg::Vec3d& position, const linalg::Vec3d& rotation, const linalg::Vec3d& scale);

  Transform(const Transform&)            = delete;
  Transform& operator=(const Transform&) = delete;
  Transform(Transform&&)                 = delete;
  Transform& operator=(Transform&&)      = delete;

  /**
   * @brief Gets the observer that notifies when the transformation changes.
   * @return The observer for transformation changes.
   */
  Observer<>& getTransformationChangedObserver() { return m_transformation_changed_observer; }

  /**
   * @brief Sets the position of the object.
   * @param pos The new position.
   */
  void setPosition(const linalg::Vec3d& pos);

  /**
   * @brief Sets the X component of the position.
   * @param x The new X position.
   */
  void setPositionX(double x);

  /**
   * @brief Sets the Y component of the position.
   * @param y The new Y position.
   */
  void setPositionY(double y);

  /**
   * @brief Sets the Z component of the position.
   * @param z The new Z position.
   */
  void setPositionZ(double z);

  /**
   * @brief Sets the rotation of the object.
   * @param rot The new rotation in Euler angles (degrees).
   */
  void setRotation(const linalg::Vec3d& rot);

  /**
   * @brief Sets the X component of the rotation.
   * @param x The new X rotation (in degrees).
   */
  void setRotationX(double x);

  /**
   * @brief Sets the Y component of the rotation.
   * @param y The new Y rotation (in degrees).
   */
  void setRotationY(double y);

  /**
   * @brief Sets the Z component of the rotation.
   * @param z The new Z rotation (in degrees).
   */
  void setRotationZ(double z);

  /**
   * @brief Sets the scale of the object.
   * @param scale The new scale.
   */
  void setScale(const linalg::Vec3d& scale);

  /**
   * @brief Sets the scale of the object.
   * @param scale The new uniform scale.
   */
  void setScale(double scale);

  /**
   * @brief Sets the X component of the scale.
   * @param x The new X scale.
   */
  void setScaleX(double x);

  /**
   * @brief Sets the Y component of the scale.
   * @param y The new Y scale.
   */
  void setScaleY(double y);

  /**
   * @brief Sets the Z component of the scale.
   * @param z The new Z scale.
   */
  void setScaleZ(double z);

  /**
   * @brief Gets the current position of the object.
   * @return The position vector.
   */
  linalg::Vec3d getPosition() const { return m_position; }

  /**
   * @brief Gets the current rotation of the object in radians (converted from degrees).
   * @return The rotation vector in radians.
   */
  linalg::Vec3d getRotation() const { return m_rotation * DEG_TO_RAD; }

  /**
   * @brief Gets the current scale of the object.
   * @return The scale vector.
   */
  linalg::Vec3d getScale() const { return m_scale; }

  /**
   * @brief Gets the current translation matrix.
   * @return The translation matrix.
   */
  linalg::Mat4d getTranslationMatrix() const { return m_translation_matrix; }

  /**
   * @brief Gets the current rotation matrix.
   * @return The rotation matrix.
   */
  linalg::Mat3d getRotationMatrix() const { return m_rotation_matrix; }

  /**
   * @brief Gets the current scale matrix.
   * @return The scale matrix.
   */
  linalg::Mat3d getScaleMatrix() const { return m_scale_matrix; }

  /**
   * @brief Gets the final transformation matrix.
   * @return The transformation matrix.
   */
  linalg::Mat4d getTransformationMatrix() const { return m_transformation_matrix; }

  /**
   * @brief Gets the inverse of the transformation matrix.
   * @return The inverse transformation matrix.
   */
  linalg::Mat4d getInverseMatrix() const { return m_inverse_matrix; }

  /**
   * @brief Gets the normal matrix for transforming normals.
   * @return The normal matrix.
   */
  linalg::Mat3d getNormalMatrix() const { return m_normal_matrix; }

  ~Transform() = default; ///< Default destructor.
};

#endif // CORE_TRANSFORM_HPP