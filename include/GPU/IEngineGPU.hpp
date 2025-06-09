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
  float m_camera_move_speed   = DEFAULT_CAMERA_MOVE_SPEED;
  float m_camera_rotate_speed = DEFAULT_CAMERA_ROTATE_SPEED;
  float m_camera_zoom_speed   = DEFAULT_CAMERA_ZOOM_SPEED;

protected:
  float cameraMoveSpeed() const { return m_camera_move_speed; }
  float cameraRotateSpeed() const { return m_camera_rotate_speed; }
  float cameraZoomSpeed() const { return m_camera_zoom_speed; }

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
  void setCameraRotateSpeed(float speed) {
    m_camera_rotate_speed = std::clamp(speed, MIN_CAMERA_MOVE_SPEED, MAX_CAMERA_MOVE_SPEED);
  }

  /**
   * @brief Sets the camera zoom speed.
   * @param speed The desired camera zoom speed.
   */
  void setCameraZoomSpeed(float speed) {
    m_camera_zoom_speed = std::clamp(speed, MIN_CAMERA_MOVE_SPEED, MAX_CAMERA_MOVE_SPEED);
  }

  virtual void render() = 0; ///< Pure virtual function to render the scene using the GPU engine.

  virtual ~IEngineGPU() = default; ///< Virtual destructor for the GPU engine interface.
};

#endif // GPU_IENGINEGPU_HPP