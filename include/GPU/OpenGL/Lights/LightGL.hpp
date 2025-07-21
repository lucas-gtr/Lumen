#ifndef GPU_OPENGL_LIGHTS_LIGHTGL_HPP
#define GPU_OPENGL_LIGHTS_LIGHTGL_HPP

#include <linalg/Mat4.hpp>
#include <vector>

#include "Core/Config.hpp"

/**
 * @class LightGL
 * @brief Abstract base class for shadow-casting OpenGL lights.
 */
class LightGL {
private:
  float m_near_plane = DEFAULT_LIGHT_NEAR_PLANE;
  float m_far_plane  = DEFAULT_LIGHT_FAR_PLANE;

protected:
  void setFarPlane(float far_plane) { m_far_plane = far_plane; }

public:
  LightGL() = default;

  LightGL(const LightGL&)            = delete;
  LightGL& operator=(const LightGL&) = delete;
  LightGL(LightGL&&)                 = delete;
  LightGL& operator=(LightGL&&)      = delete;

  /**
   * @brief Gets the light space matrix for the shadow light.
   * @return A raw pointer to the light space matrix data.
   */
  float getNearPlane() const { return m_near_plane; }

  /**
   * @brief Gets the far plane distance of the shadow light.
   * @return The far plane distance as a float.
   */
  float getFarPlane() const { return m_far_plane; }

  virtual void updateLightSpaceMatrix() = 0;

  virtual ~LightGL() = default;
};

#endif // GPU_OPENGL_LIGHTS_LIGHTGL_HPP
