/**
 * @file Transform.hpp
 * @brief Header file for the Transform class.
 */
#ifndef CORE_TRANSFORM_HPP
#define CORE_TRANSFORM_HPP

#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/MathConstants.hpp"

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
  lin::Mat4d m_transformationMatrix = lin::Mat4d::Identity();
  lin::Mat4d m_inverseMatrix        = lin::Mat4d::Identity();

  lin::Mat4d m_translationMatrix = lin::Mat4d::Identity();
  lin::Mat3d m_rotationMatrix    = lin::Mat3d::Identity();
  lin::Mat3d m_scaleMatrix       = lin::Mat3d::Identity();

  lin::Mat3d m_normalMatrix = lin::Mat3d::Identity();

  lin::Vec3d m_position;
  lin::Vec3d m_rotation;
  lin::Vec3d m_scale;

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
  Transform() : m_position(lin::Vec3d(0.0)), m_rotation(lin::Vec3d(0.0)), m_scale(lin::Vec3d(1.0)) {}

  /**
   * @brief Constructs a Transform with specific position, rotation, and scale.
   * @param position The position of the object.
   * @param rotation The rotation of the object in Euler angles (degrees).
   * @param scale The scale of the object.
   */
  Transform(const lin::Vec3d& position, const lin::Vec3d& rotation, const lin::Vec3d& scale);

  Transform(const Transform&)            = default; ///< Default copy constructor.
  Transform& operator=(const Transform&) = default; ///< Default copy assignment operator.
  Transform(Transform&&)                 = default; ///< Default move constructor.
  Transform& operator=(Transform&&)      = default; ///< Default move assignment operator.

  /**
   * @brief Sets the position of the object.
   * @param pos The new position.
   */
  void setPosition(const lin::Vec3d& pos);

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
  void setRotation(const lin::Vec3d& rot);

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
  void setScale(const lin::Vec3d& scale);

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
  lin::Vec3d getPosition() const { return m_position; }

  /**
   * @brief Gets the current rotation of the object in radians (converted from degrees).
   * @return The rotation vector in radians.
   */
  lin::Vec3d getRotation() const { return m_rotation * DEG_TO_RAD; }

  /**
   * @brief Gets the current scale of the object.
   * @return The scale vector.
   */
  lin::Vec3d getScale() const { return m_scale; }

  /**
   * @brief Gets the current translation matrix.
   * @return The translation matrix.
   */
  lin::Mat4d getTranslationMatrix() const { return m_translationMatrix; }

  /**
   * @brief Gets the current rotation matrix.
   * @return The rotation matrix.
   */
  lin::Mat3d getRotationMatrix() const { return m_rotationMatrix; }

  /**
   * @brief Gets the current scale matrix.
   * @return The scale matrix.
   */
  lin::Mat3d getScaleMatrix() const { return m_scaleMatrix; }

  /**
   * @brief Gets the final transformation matrix.
   * @return The transformation matrix.
   */
  lin::Mat4d getTransformationMatrix() const { return m_transformationMatrix; }

  /**
   * @brief Gets the inverse of the transformation matrix.
   * @return The inverse transformation matrix.
   */
  lin::Mat4d getInverseMatrix() const { return m_inverseMatrix; }

  /**
   * @brief Gets the normal matrix for transforming normals.
   * @return The normal matrix.
   */
  lin::Mat3d getNormalMatrix() const { return m_normalMatrix; }

  ~Transform() = default; ///< Default destructor.
};

#endif // CORE_TRANSFORM_HPP