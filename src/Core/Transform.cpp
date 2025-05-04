#include "Core/Transform.hpp"
#include "Core/MathConstants.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Transform::Transform(const Eigen::Vector3d& position, const Eigen::Vector3d& rotation, const Eigen::Vector3d& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {

  updateTranslationMatrix();
  updateRotationMatrix();
  updateScaleMatrix();
  updateTransformationMatrix();
}

void Transform::setPosition(const Eigen::Vector3d& pos) {
  m_position = pos;
  updateTranslationMatrix();
}

void Transform::setPositionX(double x) {
  m_position.x() = x;
  updateTranslationMatrix();
}

void Transform::setPositionY(double y) {
  m_position.y() = y;
  updateTranslationMatrix();
}

void Transform::setPositionZ(double z) {
  m_position.z() = z;
  updateTranslationMatrix();
}

void Transform::updateTranslationMatrix() {
  m_translationMatrix.setIdentity();
  m_translationMatrix(0, 3) = m_position.x();
  m_translationMatrix(1, 3) = m_position.y();
  m_translationMatrix(2, 3) = m_position.z();
  updateTransformationMatrix();
}

void Transform::setRotation(const Eigen::Vector3d& rot) {
  m_rotation = rot;
  updateRotationMatrix();
}

void Transform::setRotationX(double x) {
  m_rotation.x() = x;
  updateRotationMatrix();
}

void Transform::setRotationY(double y) {
  m_rotation.y() = y;
  updateRotationMatrix();
}

void Transform::setRotationZ(double z) {
  m_rotation.z() = z;
  updateRotationMatrix();
}

void Transform::updateRotationMatrix() {
  m_rotationMatrix = Eigen::AngleAxisd(m_rotation.z() * DEG_TO_RAD, Eigen::Vector3d::UnitZ()) *
                     Eigen::AngleAxisd(m_rotation.y() * DEG_TO_RAD, Eigen::Vector3d::UnitY()) *
                     Eigen::AngleAxisd(m_rotation.x() * DEG_TO_RAD, Eigen::Vector3d::UnitX());
  updateTransformationMatrix();
}

void Transform::setScale(const Eigen::Vector3d& scale) {
  m_scale = scale;
  updateScaleMatrix();
}

void Transform::setScale(double scale) {
  m_scale = Eigen::Vector3d(scale, scale, scale);
  updateScaleMatrix();
}

void Transform::setScaleX(double x) {
  m_scale.x() = x;
  updateScaleMatrix();
}

void Transform::setScaleY(double y) {
  m_scale.y() = y;
  updateScaleMatrix();
}

void Transform::setScaleZ(double z) {
  m_scale.z() = z;
  updateScaleMatrix();
}

void Transform::updateScaleMatrix() {
  m_scaleMatrix.setIdentity();
  m_scaleMatrix(0, 0) = m_scale.x();
  m_scaleMatrix(1, 1) = m_scale.y();
  m_scaleMatrix(2, 2) = m_scale.z();
  updateTransformationMatrix();
}

void Transform::updateTransformationMatrix() {
  Eigen::Matrix4d rotationMatrix4d   = Eigen::Matrix4d::Identity();
  rotationMatrix4d.block<3, 3>(0, 0) = m_rotationMatrix;

  Eigen::Matrix4d scaleMatrix4d   = Eigen::Matrix4d::Identity();
  scaleMatrix4d.block<3, 3>(0, 0) = m_scaleMatrix;

  m_transformationMatrix = m_translationMatrix * scaleMatrix4d * rotationMatrix4d;
  updateInverseMatrix();
  updateNormalMatrix();
}

void Transform::updateInverseMatrix() { m_inverseMatrix = m_transformationMatrix.inverse(); }

void Transform::updateNormalMatrix() {
  m_normalMatrix = m_transformationMatrix.block<3, 3>(0, 0).inverse().transpose();
}
