/**
 * @file CameraGL.hpp
 * @brief Header file for the CameraGL class, which manages camera operations in OpenGL.
 */
#ifndef GPU_OPENGL_CAMERAGL_HPP
#define GPU_OPENGL_CAMERAGL_HPP

// GCOVR_EXCL_START

#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>

#include "Core/Ray.hpp"
#include "GPU/OpenGL/UniformBufferGL.hpp"
#include "SceneObjects/Camera.hpp"

/**
 * @struct CameraUBO
 * @brief Structure representing the camera's Uniform Buffer Object (UBO) data.
 *
 * This structure contains the view and projection matrices, camera position, and other
 * relevant data that can be efficiently accessed by shaders in OpenGL.
 *
 * @note The matrices are stored in column-major order, which is the default for OpenGL.
 */
struct CameraUBO {
  linalg::Mat4f view_transposed                     = linalg::Mat4f::Identity();
  linalg::Mat4f view_without_translation_transposed = linalg::Mat4f::Identity();
  linalg::Mat4f projection_transposed               = linalg::Mat4f::Identity();
  linalg::Vec3f position                            = linalg::Vec3f(0.0F);
  float         padding1{};
};

/**
 * @class CameraGL
 * @brief Class that manages camera operations in OpenGL.
 *
 * This class provides methods to handle camera transformations, view and projection matrices,
 * and updates to the camera's position and orientation. It uses a Uniform Buffer Object (UBO) to store
 * camera-related data that can be efficiently accessed by shaders.
 */
class CameraGL {
private:
  Camera* m_render_camera;

  linalg::Vec3f m_forward;
  linalg::Vec3f m_right;
  linalg::Vec3f m_up;

  CameraUBO       m_camera_ubo;
  UniformBufferGL m_gl_camera_ubo;

  float m_yaw   = 0.0;
  float m_pitch = 0.0;

  int m_viewport_width;
  int m_viewport_height;

  void updateAxisVectors();

public:
  /**
   * @brief Constructor for CameraGL.
   * @param cam The Camera object to build the OpenGL camera from.
   * @param viewport_width The width of the viewport in pixels.
   * @param viewport_height The height of the viewport in pixels.
   */
  CameraGL(Camera& cam, int viewport_width, int viewport_height);

  /**
   * @brief Sets the viewport size.
   * @param width The width of the viewport in pixels.
   * @param height The height of the viewport in pixels.
   */
  void setViewportSize(int width, int height);

  /**
   * @brief Updates the render camera from the viewport camera position and rotation.
   */
  void updateRenderCamera();

  /**
   * @brief Updates the viewport camera based on the current render camera position and rotation.
   */
  void updateViewportCamera();

  /**
   * @brief Updates the view matrix based on the current camera position and orientation.
   */
  void updateViewMatrix();

  /**
   * @brief Updates the projection matrix based on the current camera settings.
   */
  void updateProjectionMatrix();

  /**
   * @brief Gets ID of the OpenGL Uniform Buffer Object (UBO) used for the camera.
   * @return The OpenGL ID of the camera UBO.
   */
  unsigned int getUBO() const { return m_gl_camera_ubo.getID(); }

  /**
   * @brief Moves the camera forward by a specified distance.
   * @param distance The distance to move the camera forward.
   */
  void moveForward(float distance);

  /**
   * @brief Moves the camera to the right by a specified distance.
   * @param distance The distance to move the camera to the right.
   */
  void moveRight(float distance);

  /**
   * @brief Moves the camera up by a specified distance.
   * @param distance The distance to move the camera up.
   */
  void moveUp(float distance);

  /**
   * @brief Rotates the camera by specified yaw and pitch angles.
   * @param delta_yaw The change in yaw angle (in radians).
   * @param delta_pitch The change in pitch angle (in radians).
   */
  void rotate(float delta_yaw, float delta_pitch);

  Ray getRayFromMousePosition(int mouse_x, int mouse_y) const;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_CAMERAGL_HPP