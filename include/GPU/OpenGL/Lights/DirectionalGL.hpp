/**
 * @file DirectionalGL.hpp
 * @brief Header file for the DirectionalLightGL class, which represents a directional light in OpenGL.
 */
#ifndef GPU_OPENGL_LIGHTS_DIRECTIONALGL_HPP
#define GPU_OPENGL_LIGHTS_DIRECTIONALGL_HPP

#include "Core/Config.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "GPU/Lights/DirectionalGPU.hpp"
#include "Lighting/DirectionalLight.hpp"

/**
 * @class DirectionalLightGL
 * @brief A class that represents a directional light in OpenGL, inheriting from DirectionalLightGPU.
 */
class DirectionalLightGL : public DirectionalLightGPU {
private:
  float m_near_plane = DEFAULT_LIGHT_NEAR_PLANE;
  float m_far_plane  = DEFAULT_LIGHT_FAR_PLANE;

  lin::Mat4f m_lightSpaceMatrix;

public:
  /**
   * @brief Constructor for DirectionalLightGL.
   * @param light The DirectionalLight object to initialize this DirectionalLightGL with.
   */
  explicit DirectionalLightGL(DirectionalLight* light);

  DirectionalLightGL(const DirectionalLightGL&)            = delete;
  DirectionalLightGL& operator=(const DirectionalLightGL&) = delete;
  DirectionalLightGL(DirectionalLightGL&&)                 = delete;
  DirectionalLightGL& operator=(DirectionalLightGL&&)      = delete;

  /**
   * @brief Gets the light space matrix for the directional light.
   * @return A raw pointer to the light space matrix data.
   */
  const float* getLightSpaceMatrix() const {
    return m_lightSpaceMatrix.data(); ///< Gets the light space matrix for the directional light.
  }

  /**
   * @brief Updates the light space matrix for the directional light.
   */
  void updateLightSpaceMatrix();

  ~DirectionalLightGL() = default; ///< Default destructor.
};

#endif // GPU_OPENGL_LIGHTS_DIRECTIONALGL_HPP