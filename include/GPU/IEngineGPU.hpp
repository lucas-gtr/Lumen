/**
 * @file IEngineGPU.hpp
 * @brief Header file for the IEngineGPU interface, which defines the GPU engine interface.
 */
#ifndef GPU_IENGINEGPU_HPP
#define GPU_IENGINEGPU_HPP

#include <algorithm>

#include "Core/Config.hpp"

/**
 * @class IEngineGPU
 * @brief Interface for the GPU engine, defining methods for rendering and camera control.
 *
 * This interface provides methods to set camera movement, rotation, and zoom speeds,
 * as well as a pure virtual function for rendering the scene. It is designed to be
 * implemented by specific GPU engine classes, such as OpenGL, Vulkan or CUDA.
 */
class IEngineGPU {
private:
  float m_camera_move_speed     = DEFAULT_CAMERA_MOVE_SPEED;
  float m_camera_rotation_speed = DEFAULT_CAMERA_ROTATE_SPEED;
  float m_camera_zoom_speed     = DEFAULT_CAMERA_ZOOM_SPEED;

public:
  IEngineGPU() = default; ///< Default constructor for the GPU engine interface.

  IEngineGPU(const IEngineGPU&)            = delete;
  IEngineGPU& operator=(const IEngineGPU&) = delete;
  IEngineGPU(IEngineGPU&&)                 = delete;
  IEngineGPU& operator=(IEngineGPU&&)      = delete;

  /**
   * @brief Sets the camera movement speed.
   * @param speed The desired camera movement speed.
   */
  void setCameraMoveSpeed(float speed) {
    m_camera_move_speed = std::clamp(speed, MIN_CAMERA_MOVE_SPEED, MAX_CAMERA_MOVE_SPEED);
  }

  /**
   * @brief Sets the camera rotation speed.
   * @param speed The desired camera rotation speed.
   */
  void setCameraRotationSpeed(float speed) {
    m_camera_rotation_speed = std::clamp(speed, MIN_CAMERA_ROTATION_SPEED, MAX_CAMERA_ROTATION_SPEED);
  }

  /**
   * @brief Sets the camera zoom speed.
   * @param speed The desired camera zoom speed.
   */
  void setCameraZoomSpeed(float speed) {
    m_camera_zoom_speed = std::clamp(speed, MIN_CAMERA_ZOOM_SPEED, MAX_CAMERA_ZOOM_SPEED);
  }

  /**
   * @brief Gets the current camera movement speed.
   * @return The current camera movement speed.
   */
  float cameraMoveSpeed() const { return m_camera_move_speed; }

  /**
   * @brief Gets the current camera rotation speed.
   * @return The current camera rotation speed.
   */
  float cameraRotationSpeed() const { return m_camera_rotation_speed; }

  /**
   * @brief Gets the current camera zoom speed.
   * @return The current camera zoom speed.
   */
  float cameraZoomSpeed() const { return m_camera_zoom_speed; }

  virtual ~IEngineGPU() = default; ///< Virtual destructor for the GPU engine interface.
};

#endif // GPU_IENGINEGPU_HPP