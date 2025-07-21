/**
 * @file DirectionalGPU.hpp
 * @brief Header file for the DirectionalLightGPU class, which represents a directional light for GPU memory.
 */
#ifndef GPU_LIGHTS_DIRECTIONALGPU_HPP
#define GPU_LIGHTS_DIRECTIONALGPU_HPP

#include <linalg/Vec3.hpp>

#include "GPU/Lights/LightGPU.hpp"
#include "Lighting/DirectionalLight.hpp"

/**
 * @class DirectionalLightGPU
 * @brief Represents a directional light in GPU memory.
 *
 * This class is designed to hold the properties of a directional light,
 * such as its direction and color, for efficient access in GPU rendering.
 */
class DirectionalLightGPU : public LightGPU {
private:
  const DirectionalLight* m_light = nullptr;

  linalg::Vec3f m_direction;

public:
  /**
   * @brief Constructor for DirectionalLightGPU.
   * @param light Pointer to the DirectionalLight object to be represented in GPU memory.
   */
  explicit DirectionalLightGPU(const DirectionalLight* light);

  DirectionalLightGPU(const DirectionalLightGPU&)            = delete;
  DirectionalLightGPU& operator=(const DirectionalLightGPU&) = delete;
  DirectionalLightGPU(DirectionalLightGPU&&)                 = delete;
  DirectionalLightGPU& operator=(DirectionalLightGPU&&)      = delete;

  /**
   * @brief Retrieves the direction and color of the light.
   */
  void retrieveData() override;

  /**
   * @brief Returns the direction of the light.
   * @return The direction vector of the light.
   */
  linalg::Vec3f getDirection() const { return m_direction; }

  /**
   * @brief Gets the light source.
   * @return A pointer to the DirectionalLight source.
   */
  const Light* getSource() const override { return m_light; }

  ~DirectionalLightGPU() override = default; ///< Default destructor.
};

#endif // GPU_LIGHTS_DIRECTIONALGPU_HPP