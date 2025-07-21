/**
 * @file SpotGL.hpp
 * @brief Header file for the SpotLightGL class, which represents a spot light in OpenGL.
 */
#ifndef GPU_OPENGL_LIGHTS_SPOTGL_HPP
#define GPU_OPENGL_LIGHTS_SPOTGL_HPP

#include <linalg/Vec3.hpp>

#include "GPU/Lights/SpotGPU.hpp"
#include "GPU/OpenGL/Lights/LightGL.hpp"
#include "Lighting/SpotLight.hpp"

/**
 * @class SpotLightGL
 * @brief A class that represents a spot light in OpenGL, inheriting from SpotLightGPU.
 *
 * This class extends the SpotLightGPU class to provide functionality specific to OpenGL,
 * including the management of the light space matrix and the near and far planes for shadow mapping.
 */
class SpotLightGL : public SpotLightGPU, public LightGL {
private:
  linalg::Mat4f m_light_space_matrix;

public:
  /**
   * @brief Constructor for SpotLightGL.
   * @param light The SpotLight object to initialize this SpotLightGL with.
   */
  explicit SpotLightGL(SpotLight* light);

  SpotLightGL(const SpotLightGL&)            = delete;
  SpotLightGL& operator=(const SpotLightGL&) = delete;
  SpotLightGL(SpotLightGL&&)                 = delete;
  SpotLightGL& operator=(SpotLightGL&&)      = delete;

  /**
   * @brief Gets the light space matrix of the spot light.
   * @return A raw pointer to the light space matrix data.
   */
  const float* getLightSpaceMatrix() const { return m_light_space_matrix.data(); }

  /**
   * @brief Updates the light space matrix based on the light's position, direction and angles.
   */
  void updateLightSpaceMatrix() override;

  ~SpotLightGL() override = default; ///< Default destructor.
};

#endif // GPU_OPENGL_LIGHTS_SPOTGL_HPP