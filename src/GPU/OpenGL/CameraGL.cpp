// GCOVR_EXCL_START
#include <cmath>
#include <cstddef>

#include "Core/Config.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Core/MathConstants.hpp"
#include "GPU/OpenGL/CameraGL.hpp"
#include "SceneObjects/Camera.hpp"

CameraGL::CameraGL(Camera& cam, int viewportWidth, int viewportHeight)
    : m_render_camera(&cam), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight),
      m_glCameraUBO(sizeof(CameraUBO), CAMERA_UBO_BINDING_POINT) {

  m_cameraUBO.position = lin::Vec3f(m_render_camera->getPosition());
  m_pitch              = static_cast<float>(m_render_camera->getRotation().x);
  m_yaw                = static_cast<float>(m_render_camera->getRotation().y);

  updateAxisVectors();
  updateProjectionMatrix();
}

void CameraGL::setViewportSize(int width, int height) {
  m_viewportWidth  = width;
  m_viewportHeight = height;
  updateProjectionMatrix();
}

void CameraGL::rotate(float deltaYaw, float deltaPitch) {
  m_yaw += deltaYaw;
  m_pitch += deltaPitch;

  if(m_pitch > MAX_CAMERA_PITCH) {
    m_pitch = MAX_CAMERA_PITCH;
  } else if(m_pitch < MIN_CAMERA_PITCH) {
    m_pitch = MIN_CAMERA_PITCH;
  }

  m_yaw = static_cast<float>(std::fmod(m_yaw, TWO_PI));

  updateAxisVectors();
}

void CameraGL::updateAxisVectors() {
  static constexpr lin::Vec3f worldUp = {0.0F, 1.0F, 0.0F};

  const float cosPitch = std::cos(m_pitch);
  const float sinPitch = std::sin(m_pitch);
  const float cosYaw   = std::cos(m_yaw);
  const float sinYaw   = std::sin(m_yaw);

  m_forward = {-cosPitch * sinYaw, sinPitch, -cosPitch * cosYaw};
  m_forward.normalize();

  m_right = m_forward.cross(worldUp);
  m_right.normalize();

  // Since m_right and m_forward are orthogonal and normalized, m_up will also be normalized.
  m_up = m_right.cross(m_forward);

  updateViewMatrix();
}

void CameraGL::moveForward(float distance) {
  m_cameraUBO.position += m_forward * distance;
  m_glCameraUBO.updateData(&m_cameraUBO.position, sizeof(lin::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

void CameraGL::moveRight(float distance) {
  m_cameraUBO.position += m_right * distance;
  m_glCameraUBO.updateData(&m_cameraUBO.position, sizeof(lin::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

void CameraGL::moveUp(float distance) {
  m_cameraUBO.position += m_up * distance;
  m_glCameraUBO.updateData(&m_cameraUBO.position, sizeof(lin::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

// void CameraGL::updateRenderCamera() { std::cout << "Not implemented: CameraGL::updateRenderCamera()" << '\n'; }

void CameraGL::updateViewMatrix() {
  m_cameraUBO.view = lin::Mat4f::Identity();

  m_cameraUBO.view(0, 0) = m_right.x;
  m_cameraUBO.view(1, 0) = m_right.y;
  m_cameraUBO.view(2, 0) = m_right.z;
  m_cameraUBO.view(3, 0) = -lin::dot(m_right, m_cameraUBO.position);
  m_cameraUBO.view(0, 1) = m_up.x;
  m_cameraUBO.view(1, 1) = m_up.y;
  m_cameraUBO.view(2, 1) = m_up.z;
  m_cameraUBO.view(3, 1) = -lin::dot(m_up, m_cameraUBO.position);
  m_cameraUBO.view(0, 2) = -m_forward.x;
  m_cameraUBO.view(1, 2) = -m_forward.y;
  m_cameraUBO.view(2, 2) = -m_forward.z;
  m_cameraUBO.view(3, 2) = lin::dot(m_forward, m_cameraUBO.position);
  m_cameraUBO.view(0, 3) = 0.0F;
  m_cameraUBO.view(1, 3) = 0.0F;
  m_cameraUBO.view(2, 3) = 0.0F;
  m_cameraUBO.view(3, 3) = 1.0F;

  m_cameraUBO.viewWithoutTranslation = m_cameraUBO.view;

  m_cameraUBO.viewWithoutTranslation(3, 0) = 0.0F;
  m_cameraUBO.viewWithoutTranslation(3, 1) = 0.0F;
  m_cameraUBO.viewWithoutTranslation(3, 2) = 0.0F;

  m_glCameraUBO.updateData(&m_cameraUBO.view, sizeof(lin::Mat4f), offsetof(CameraUBO, view));
  m_glCameraUBO.updateData(&m_cameraUBO.viewWithoutTranslation, sizeof(lin::Mat4f),
                           offsetof(CameraUBO, viewWithoutTranslation));
}

void CameraGL::updateProjectionMatrix() {
  const auto aspectRatio = static_cast<double>(m_viewportWidth) / static_cast<double>(m_viewportHeight);
  const auto fovX        = m_render_camera->getFovX() * DEG_TO_RAD;
  const auto fovY        = static_cast<float>(2.0 * std::atan(std::tan(fovX * HALF) / aspectRatio));

  const auto nearPlane = static_cast<float>(m_render_camera->getNearPlane());
  const auto farPlane  = static_cast<float>(m_render_camera->getFarPlane());

  m_cameraUBO.projection =
      lin::Mat4f::Perspective(fovY, static_cast<float>(aspectRatio), nearPlane, farPlane).transposed();

  m_glCameraUBO.updateData(&m_cameraUBO.projection, sizeof(lin::Mat4f), offsetof(CameraUBO, projection));
}

// GCOVR_EXCL_STOP