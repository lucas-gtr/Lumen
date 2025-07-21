/**
 * @file DirectionalGL.hpp
 * @brief Header file for the DirectionalLightGL class, which represents a directional light in OpenGL.
 */
#ifndef GPU_OPENGL_LIGHTS_DIRECTIONALGL_HPP
#define GPU_OPENGL_LIGHTS_DIRECTIONALGL_HPP

#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>

#include "Core/Config.hpp"
#include "GPU/Lights/DirectionalGPU.hpp"
#include "GPU/OpenGL/Lights/LightGL.hpp"
#include "Lighting/DirectionalLight.hpp"

/**
 * @class DirectionalLightGL
 * @brief A class that represents a directional light in OpenGL, inheriting from DirectionalLightGPU.
 */
class DirectionalLightGL : public DirectionalLightGPU, public LightGL {
private:
  linalg::Mat4f m_light_space_matrix;

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
  const float* getLightSpaceMatrix() const { return m_light_space_matrix.data(); }

  /**
   * @brief Updates the light space matrix for the directional light.
   */
  void updateLightSpaceMatrix() override;

  ~DirectionalLightGL() override = default; ///< Default destructor.
};

#endif // GPU_OPENGL_LIGHTS_DIRECTIONALGL_HPP