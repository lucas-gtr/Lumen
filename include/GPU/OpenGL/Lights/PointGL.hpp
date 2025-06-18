/**
 * @file PointGL.hpp
 * @brief Header file for the PointLightGL class, which represents a point light in OpenGL.
 */
#ifndef GPU_OPENGL_LIGHTS_POINTGL_HPP
#define GPU_OPENGL_LIGHTS_POINTGL_HPP

#include <vector>

#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "GPU/Lights/PointGPU.hpp"
#include "Lighting/PointLight.hpp"

/**
 * @class PointLightGL
 * @brief A class that represents a point light in OpenGL, inheriting from PointLightGPU.
 *
 * This class extends the PointLightGPU class to provide functionality specific to OpenGL,
 * including the management of light space matrices for shadow mapping and the near and far planes.
 */
class PointLightGL : public PointLightGPU {
private:
  std::vector<lin::Mat4f> m_lightSpaceMatrices;
  float                   m_near_plane = DEFAULT_LIGHT_NEAR_PLANE;
  float                   m_far_plane  = DEFAULT_LIGHT_FAR_PLANE;

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
  const float* getLightSpaceMatrix(int index) const { return m_lightSpaceMatrices[index].data(); }

  /**
   * @brief Gets the far plane distance for the light.
   * @return The far plane distance as a float.
   */
  float getFarPlane() const { return m_far_plane; }

  /**
   * @brief Updates the light space matrices for the point light.
   */
  void updateLightSpaceMatrices();

  ~PointLightGL() = default; ///< Default destructor.
};

#endif // GPU_OPENGL_LIGHTS_POINTGL_HPP