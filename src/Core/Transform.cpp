#include <linalg/Mat3.hpp>
#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "Core/MathConstants.hpp"
#include "Core/Transform.hpp"

#include <cmath>

Transform::Transform(const linalg::Vec3d& position, const linalg::Vec3d& rotation, const linalg::Vec3d& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {

  updateTranslationMatrix();
  updateRotationMatrix();
  updateScaleMatrix();
  updateTransformationMatrix();
}

void Transform::setPosition(const linalg::Vec3d& pos) {
  m_position = pos;
  updateTranslationMatrix();
}

void Transform::setPositionX(double x) {
  m_position.x = x;
  updateTranslationMatrix();
}

void Transform::setPositionY(double y) {
  m_position.y = y;
  updateTranslationMatrix();
}

void Transform::setPositionZ(double z) {
  m_position.z = z;
  updateTranslationMatrix();
}

void Transform::updateTranslationMatrix() {
  m_translation_matrix(0, 3) = m_position.x;
  m_translation_matrix(1, 3) = m_position.y;
  m_translation_matrix(2, 3) = m_position.z;
  updateTransformationMatrix();
}

void Transform::setRotation(const linalg::Vec3d& rot) {
  m_rotation = rot;
  updateRotationMatrix();
}

void Transform::setRotationX(double x) {
  m_rotation.x = x;
  updateRotationMatrix();
}

void Transform::setRotationY(double y) {
  m_rotation.y = y;
  updateRotationMatrix();
}

void Transform::setRotationZ(double z) {
  m_rotation.z = z;
  updateRotationMatrix();
}

void Transform::updateRotationMatrix() {
  m_rotation_matrix =
      linalg::getRotationMatrix(m_rotation.x * DEG_TO_RAD, m_rotation.y * DEG_TO_RAD, m_rotation.z * DEG_TO_RAD);
  updateTransformationMatrix();
}

void Transform::setScale(const linalg::Vec3d& scale) {
  m_scale = scale;
  updateScaleMatrix();
}

void Transform::setScale(double scale) {
  m_scale = linalg::Vec3d(scale, scale, scale);
  updateScaleMatrix();
}

void Transform::setScaleX(double x) {
  m_scale.x = x;
  updateScaleMatrix();
}

void Transform::setScaleY(double y) {
  m_scale.y = y;
  updateScaleMatrix();
}

void Transform::setScaleZ(double z) {
  m_scale.z = z;
  updateScaleMatrix();
}

void Transform::updateScaleMatrix() {
  m_scale_matrix(0, 0) = m_scale.x;
  m_scale_matrix(1, 1) = m_scale.y;
  m_scale_matrix(2, 2) = m_scale.z;
  updateTransformationMatrix();
}

void Transform::updateTransformationMatrix() {
  const linalg::Mat4d rotation_matrix4d(m_rotation_matrix);

  const linalg::Mat4d scale_matrix4d(m_scale_matrix);

  m_transformation_matrix = m_translation_matrix * scale_matrix4d * rotation_matrix4d;
  updateInverseMatrix();
  updateNormalMatrix();

  m_transformation_changed_observer.notify();
}

void Transform::updateInverseMatrix() { m_inverse_matrix = m_transformation_matrix.inverse(); }

void Transform::updateNormalMatrix() { m_normal_matrix = m_transformation_matrix.topLeft3x3().inverse().transposed(); }
