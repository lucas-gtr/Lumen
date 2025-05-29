#include "Core/Transform.hpp"
#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Core/MathConstants.hpp"

#include <cmath>

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Transform::Transform(const lin::Vec3& position, const lin::Vec3& rotation, const lin::Vec3& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {

  updateTranslationMatrix();
  updateRotationMatrix();
  updateScaleMatrix();
  updateTransformationMatrix();
}

void Transform::setPosition(const lin::Vec3& pos) {
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
  m_translationMatrix(0, 3) = m_position.x;
  m_translationMatrix(1, 3) = m_position.y;
  m_translationMatrix(2, 3) = m_position.z;
  updateTransformationMatrix();
}

void Transform::setRotation(const lin::Vec3& rot) {
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
  m_rotationMatrix =
      lin::getRotationMatrix(m_rotation.x * DEG_TO_RAD, m_rotation.y * DEG_TO_RAD, m_rotation.z * DEG_TO_RAD);
  updateTransformationMatrix();
}

void Transform::setScale(const lin::Vec3& scale) {
  m_scale = scale;
  updateScaleMatrix();
}

void Transform::setScale(double scale) {
  m_scale = lin::Vec3(scale, scale, scale);
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
  m_scaleMatrix(0, 0) = m_scale.x;
  m_scaleMatrix(1, 1) = m_scale.y;
  m_scaleMatrix(2, 2) = m_scale.z;
  updateTransformationMatrix();
}

void Transform::updateTransformationMatrix() {
  lin::Mat4 rotationMatrix4d(m_rotationMatrix);

  lin::Mat4 scaleMatrix4d(m_scaleMatrix);

  m_transformationMatrix = m_translationMatrix * scaleMatrix4d * rotationMatrix4d;
  updateInverseMatrix();
  updateNormalMatrix();
}

void Transform::updateInverseMatrix() { m_inverseMatrix = m_transformationMatrix.inverse(); }

void Transform::updateNormalMatrix() { m_normalMatrix = m_transformationMatrix.topLeft3x3().inverse().transpose(); }
