// GCOVR_EXCL_START
#include <cmath>
#include <cstddef>
#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/Vec4.hpp>
#include <linalg/linalg.hpp>

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Ray.hpp"
#include "GPU/OpenGL/CameraGL.hpp"
#include "SceneObjects/Camera.hpp"

CameraGL::CameraGL(Camera& cam, int viewport_width, int viewport_height)
    : m_render_camera(&cam), m_viewport_width(viewport_width), m_viewport_height(viewport_height),
      m_gl_camera_ubo(sizeof(CameraUBO), CAMERA_UBO_BINDING_POINT) {

  updateViewportCamera();
  updateProjectionMatrix();

  m_render_camera->getCameraChangedObserver().add([this]() { updateProjectionMatrix(); });
}

void CameraGL::setViewportSize(int width, int height) {
  m_viewport_width  = width;
  m_viewport_height = height;
  updateProjectionMatrix();
}

void CameraGL::rotate(float delta_yaw, float delta_pitch) {
  m_yaw += delta_yaw;
  m_pitch += delta_pitch;

  if(m_pitch > MAX_CAMERA_PITCH) {
    m_pitch = MAX_CAMERA_PITCH;
  } else if(m_pitch < MIN_CAMERA_PITCH) {
    m_pitch = MIN_CAMERA_PITCH;
  }

  m_yaw = static_cast<float>(std::fmod(m_yaw, TWO_PI));

  updateAxisVectors();
}

void CameraGL::updateViewportCamera() {
  m_camera_ubo.position = linalg::Vec3f(m_render_camera->getPosition());
  m_pitch               = static_cast<float>(m_render_camera->getRotationRad().x);
  m_yaw                 = static_cast<float>(m_render_camera->getRotationRad().y);

  updateAxisVectors();
}

void CameraGL::updateRenderCamera() {
  m_render_camera->setPosition(linalg::Vec3d(m_camera_ubo.position));
  m_render_camera->setRotationRad({static_cast<double>(m_pitch), static_cast<double>(m_yaw), 0.0F});
}

void CameraGL::updateAxisVectors() {
  static constexpr linalg::Vec3f world_up = {0.0F, 1.0F, 0.0F};

  const float cos_pitch = std::cos(m_pitch);
  const float sin_pitch = std::sin(m_pitch);
  const float cos_yaw   = std::cos(m_yaw);
  const float sin_yaw   = std::sin(m_yaw);

  m_forward = {-cos_pitch * sin_yaw, sin_pitch, -cos_pitch * cos_yaw};
  m_forward.normalize();

  m_right = m_forward.cross(world_up);
  m_right.normalize();

  m_up = m_right.cross(m_forward);

  updateViewMatrix();
}

void CameraGL::moveForward(float distance) {
  m_camera_ubo.position += m_forward * distance;
  m_gl_camera_ubo.updateData(&m_camera_ubo.position, sizeof(linalg::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

void CameraGL::moveRight(float distance) {
  m_camera_ubo.position += m_right * distance;
  m_gl_camera_ubo.updateData(&m_camera_ubo.position, sizeof(linalg::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

void CameraGL::moveUp(float distance) {
  m_camera_ubo.position += m_up * distance;
  m_gl_camera_ubo.updateData(&m_camera_ubo.position, sizeof(linalg::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

// void CameraGL::updateRenderCamera() { std::cout << "Not implemented: CameraGL::updateRenderCamera()" << '\n'; }

void CameraGL::updateViewMatrix() {
  m_camera_ubo.view_transposed = linalg::Mat4f::Identity();

  m_camera_ubo.view_transposed(0, 0) = m_right.x;
  m_camera_ubo.view_transposed(1, 0) = m_right.y;
  m_camera_ubo.view_transposed(2, 0) = m_right.z;
  m_camera_ubo.view_transposed(3, 0) = -linalg::dot(m_right, m_camera_ubo.position);
  m_camera_ubo.view_transposed(0, 1) = m_up.x;
  m_camera_ubo.view_transposed(1, 1) = m_up.y;
  m_camera_ubo.view_transposed(2, 1) = m_up.z;
  m_camera_ubo.view_transposed(3, 1) = -linalg::dot(m_up, m_camera_ubo.position);
  m_camera_ubo.view_transposed(0, 2) = -m_forward.x;
  m_camera_ubo.view_transposed(1, 2) = -m_forward.y;
  m_camera_ubo.view_transposed(2, 2) = -m_forward.z;
  m_camera_ubo.view_transposed(3, 2) = linalg::dot(m_forward, m_camera_ubo.position);
  m_camera_ubo.view_transposed(0, 3) = 0.0F;
  m_camera_ubo.view_transposed(1, 3) = 0.0F;
  m_camera_ubo.view_transposed(2, 3) = 0.0F;
  m_camera_ubo.view_transposed(3, 3) = 1.0F;

  m_camera_ubo.view_without_translation_transposed = m_camera_ubo.view_transposed;

  m_camera_ubo.view_without_translation_transposed(3, 0) = 0.0F;
  m_camera_ubo.view_without_translation_transposed(3, 1) = 0.0F;
  m_camera_ubo.view_without_translation_transposed(3, 2) = 0.0F;

  m_gl_camera_ubo.updateData(&m_camera_ubo.view_transposed, sizeof(linalg::Mat4f),
                             offsetof(CameraUBO, view_transposed));
  m_gl_camera_ubo.updateData(&m_camera_ubo.view_without_translation_transposed, sizeof(linalg::Mat4f),
                             offsetof(CameraUBO, view_without_translation_transposed));
}

void CameraGL::updateProjectionMatrix() {
  const auto aspect_ratio = static_cast<double>(m_viewport_width) / static_cast<double>(m_viewport_height);
  const auto fov_x        = m_render_camera->getFovX() * DEG_TO_RAD;
  const auto fov_y        = static_cast<float>(2.0 * std::atan(std::tan(fov_x * HALF) / aspect_ratio));

  const auto near_plane = static_cast<float>(m_render_camera->getNearPlane());
  const auto far_plane  = static_cast<float>(m_render_camera->getFarPlane());

  m_camera_ubo.projection_transposed =
      linalg::Mat4f::Perspective(fov_y, static_cast<float>(aspect_ratio), near_plane, far_plane).transposed();

  m_gl_camera_ubo.updateData(&m_camera_ubo.projection_transposed, sizeof(linalg::Mat4f),
                             offsetof(CameraUBO, projection_transposed));
}

Ray CameraGL::getRayFromMousePosition(int mouse_x, int mouse_y) const {
  const float ndc_x = static_cast<float>(2 * mouse_x) / static_cast<float>(m_viewport_width) - 1.0F;
  const float ndc_y =
      1.0F - static_cast<float>(2 * mouse_y) / static_cast<float>(m_viewport_height); // Invert y-axis for NDC

  const linalg::Vec4f ndc_center(ndc_x, ndc_y, 0.0F, 1.0F);

  const linalg::Mat4f inv_proj = m_camera_ubo.projection_transposed.transposed().inverse();
  const linalg::Mat4f inv_view = m_camera_ubo.view_transposed.transposed().inverse();

  linalg::Vec4f eye_coords = inv_proj * ndc_center;
  eye_coords.z             = -1.0F;
  eye_coords.w             = 0.0F;

  const linalg::Vec4f world_coords  = inv_view * eye_coords;
  const linalg::Vec3f ray_direction = linalg::Vec3f(world_coords.x, world_coords.y, world_coords.z).normalized();

  return Ray::FromDirection(linalg::Vec3d(m_camera_ubo.position), linalg::Vec3d(ray_direction));
}

// GCOVR_EXCL_STOP