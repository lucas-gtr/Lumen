/**
 * @file PointGL.hpp
 * @brief Header file for the PointLightGL class, which represents a point light in OpenGL.
 */
#ifndef GPU_OPENGL_LIGHTS_POINTGL_HPP
#define GPU_OPENGL_LIGHTS_POINTGL_HPP

#include <vector>

#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>

#include "GPU/Lights/PointGPU.hpp"
#include "GPU/OpenGL/Lights/LightGL.hpp"
#include "Lighting/PointLight.hpp"

/**
 * @class PointLightGL
 * @brief A class that represents a point light in OpenGL, inheriting from PointLightGPU.
 *
 * This class extends the PointLightGPU class to provide functionality specific to OpenGL,
 * including the management of light space matrices for shadow mapping and the near and far planes.
 */
class PointLightGL : public PointLightGPU, public LightGL {
private:
  std::vector<linalg::Mat4f> m_light_space_matrices;

public:
  /**
   * @brief Constructor for PointLightGL.
   * @param light The PointLight object to initialize this PointLightGL with.
   */
  explicit PointLightGL(PointLight* light);

  PointLightGL(const PointLightGL&)            = delete;
  PointLightGL& operator=(const PointLightGL&) = delete;
  PointLightGL(PointLightGL&&)                 = delete;
  PointLightGL& operator=(PointLightGL&&)      = delete;

  /**
   * @brief Gets the light space matrix for a specific index (direction).
   * @param index The index of the light space matrix to retrieve.
   * @return A raw pointer to the light space matrix data.
   */
  const float* getLightSpaceMatrix(int index) const { return m_light_space_matrices[index].data(); }

  /**
   * @brief Updates the light space matrices for the point light.
   */
  void updateLightSpaceMatrix() override;

  ~PointLightGL() override = default; ///< Default destructor.
};

#endif // GPU_OPENGL_LIGHTS_POINTGL_HPP