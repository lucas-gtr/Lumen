// GCOVR_EXCL_START
#include <cmath>
#include <cstddef>

#include "Core/Config.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/Math/lin.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Ray.hpp"
#include "GPU/OpenGL/CameraGL.hpp"
#include "SceneObjects/Camera.hpp"

CameraGL::CameraGL(Camera& cam, int viewportWidth, int viewportHeight)
    : m_render_camera(&cam), m_viewport_width(viewportWidth), m_viewport_height(viewportHeight),
      m_gl_camera_UBO(sizeof(CameraUBO), CAMERA_UBO_BINDING_POINT) {

  updateViewportCamera();
  updateProjectionMatrix();

  m_render_camera->getCameraChangedObserver().add([this]() { updateProjectionMatrix(); });
}

void CameraGL::setViewportSize(int width, int height) {
  m_viewport_width  = width;
  m_viewport_height = height;
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

void CameraGL::updateViewportCamera() {
  m_camera_UBO.position = lin::Vec3f(m_render_camera->getPosition());
  m_pitch               = static_cast<float>(m_render_camera->getRotation().x);
  m_yaw                 = static_cast<float>(m_render_camera->getRotation().y);

  updateAxisVectors();
}

void CameraGL::updateRenderCamera() {
  m_render_camera->setPosition(lin::Vec3d(m_camera_UBO.position));
  m_render_camera->setRotation(
      {static_cast<double>(m_pitch) * RAD_TO_DEG, static_cast<double>(m_yaw) * RAD_TO_DEG, 0.0F});
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

  m_up = m_right.cross(m_forward);

  updateViewMatrix();
}

void CameraGL::moveForward(float distance) {
  m_camera_UBO.position += m_forward * distance;
  m_gl_camera_UBO.updateData(&m_camera_UBO.position, sizeof(lin::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

void CameraGL::moveRight(float distance) {
  m_camera_UBO.position += m_right * distance;
  m_gl_camera_UBO.updateData(&m_camera_UBO.position, sizeof(lin::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

void CameraGL::moveUp(float distance) {
  m_camera_UBO.position += m_up * distance;
  m_gl_camera_UBO.updateData(&m_camera_UBO.position, sizeof(lin::Vec3f), offsetof(CameraUBO, position));
  updateViewMatrix();
}

// void CameraGL::updateRenderCamera() { std::cout << "Not implemented: CameraGL::updateRenderCamera()" << '\n'; }

void CameraGL::updateViewMatrix() {
  m_camera_UBO.view_transposed = lin::Mat4f::Identity();

  m_camera_UBO.view_transposed(0, 0) = m_right.x;
  m_camera_UBO.view_transposed(1, 0) = m_right.y;
  m_camera_UBO.view_transposed(2, 0) = m_right.z;
  m_camera_UBO.view_transposed(3, 0) = -lin::dot(m_right, m_camera_UBO.position);
  m_camera_UBO.view_transposed(0, 1) = m_up.x;
  m_camera_UBO.view_transposed(1, 1) = m_up.y;
  m_camera_UBO.view_transposed(2, 1) = m_up.z;
  m_camera_UBO.view_transposed(3, 1) = -lin::dot(m_up, m_camera_UBO.position);
  m_camera_UBO.view_transposed(0, 2) = -m_forward.x;
  m_camera_UBO.view_transposed(1, 2) = -m_forward.y;
  m_camera_UBO.view_transposed(2, 2) = -m_forward.z;
  m_camera_UBO.view_transposed(3, 2) = lin::dot(m_forward, m_camera_UBO.position);
  m_camera_UBO.view_transposed(0, 3) = 0.0F;
  m_camera_UBO.view_transposed(1, 3) = 0.0F;
  m_camera_UBO.view_transposed(2, 3) = 0.0F;
  m_camera_UBO.view_transposed(3, 3) = 1.0F;

  m_camera_UBO.view_without_translation_transposed = m_camera_UBO.view_transposed;

  m_camera_UBO.view_without_translation_transposed(3, 0) = 0.0F;
  m_camera_UBO.view_without_translation_transposed(3, 1) = 0.0F;
  m_camera_UBO.view_without_translation_transposed(3, 2) = 0.0F;

  m_gl_camera_UBO.updateData(&m_camera_UBO.view_transposed, sizeof(lin::Mat4f), offsetof(CameraUBO, view_transposed));
  m_gl_camera_UBO.updateData(&m_camera_UBO.view_without_translation_transposed, sizeof(lin::Mat4f),
                             offsetof(CameraUBO, view_without_translation_transposed));
}

void CameraGL::updateProjectionMatrix() {
  const auto aspectRatio = static_cast<double>(m_viewport_width) / static_cast<double>(m_viewport_height);
  const auto fovX        = m_render_camera->getFovX() * DEG_TO_RAD;
  const auto fovY        = static_cast<float>(2.0 * std::atan(std::tan(fovX * HALF) / aspectRatio));

  const auto nearPlane = static_cast<float>(m_render_camera->getNearPlane());
  const auto farPlane  = static_cast<float>(m_render_camera->getFarPlane());

  m_camera_UBO.projection_transposed =
      lin::Mat4f::Perspective(fovY, static_cast<float>(aspectRatio), nearPlane, farPlane).transposed();

  m_gl_camera_UBO.updateData(&m_camera_UBO.projection_transposed, sizeof(lin::Mat4f),
                             offsetof(CameraUBO, projection_transposed));
}

Ray CameraGL::getRayFromMousePosition(int mouse_x, int mouse_y) const {
  const float ndc_x = static_cast<float>(2 * mouse_x) / static_cast<float>(m_viewport_width) - 1.0F;
  const float ndc_y =
      1.0F - static_cast<float>(2 * mouse_y) / static_cast<float>(m_viewport_height); // Invert y-axis for NDC

  const lin::Vec4f ndc_center(ndc_x, ndc_y, 0.0F, 1.0F);

  const lin::Mat4f inv_proj = m_camera_UBO.projection_transposed.transposed().inverse();
  const lin::Mat4f inv_view = m_camera_UBO.view_transposed.transposed().inverse();

  lin::Vec4f eye_coords = inv_proj * ndc_center;
  eye_coords.z          = -1.0F;
  eye_coords.w          = 0.0F;

  const lin::Vec4f world_coords  = inv_view * eye_coords;
  const lin::Vec3f ray_direction = lin::Vec3f(world_coords.x, world_coords.y, world_coords.z).normalized();

  return Ray::FromDirection(lin::Vec3d(m_camera_UBO.position), lin::Vec3d(ray_direction));
}

// GCOVR_EXCL_STOP