/**
 * @file PointGPU.hpp
 * @brief Header file for the PointLightGPU class, which represents a point light for GPU memory.
 */
#ifndef GPU_LIGHTS_POINTGPU_HPP
#define GPU_LIGHTS_POINTGPU_HPP

#include <linalg/Vec3.hpp>

#include "GPU/Lights/LightGPU.hpp"
#include "Lighting/PointLight.hpp"

/**
 * @class PointLightGPU
 * @brief Represents a point light in GPU memory.
 *
 * This class is designed to hold the properties of a point light,
 * such as its position and color, for efficient access in GPU rendering.
 */
class PointLightGPU : public LightGPU {
private:
  const PointLight* m_light = nullptr;

  linalg::Vec3f m_position;

protected:
  const PointLight* light() const { return m_light; }

public:
  /**
   * @brief Constructor for PointLightGPU.
   * @param light Pointer to the PointLight object to be represented in GPU memory.
   */
  explicit PointLightGPU(const PointLight* light);

  PointLightGPU(const PointLightGPU&)            = delete;
  PointLightGPU& operator=(const PointLightGPU&) = delete;
  PointLightGPU(PointLightGPU&&)                 = delete;
  PointLightGPU& operator=(PointLightGPU&&)      = delete;

  /**
   * @brief Retrieves the position and color of the light.
   */
  void retrieveData() override;

  /**
   * @brief Gets the position of the light.
   * @return The position vector of the light.
   */
  linalg::Vec3f getPosition() const {
    return m_position; ///< Gets the direction of the light.
  }

  /**
   * @brief Gets the light source.
   * @return A pointer to the DirectionalLight source.
   */
  const Light* getSource() const override { return m_light; }

  ~PointLightGPU() override = default; ///< Default destructor for PointLightGPU.
};

#endif // GPU_LIGHTS_POINTGPU_HPP