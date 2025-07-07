/**
 * @file SpotGPU.hpp
 * @brief Header file for the SpotLightGPU class, which represents a spot light for GPU memory.
 */
#ifndef GPU_LIGHTS_SPOTGPU_HPP
#define GPU_LIGHTS_SPOTGPU_HPP

#include <linalg/Vec3.hpp>

#include "Lighting/SpotLight.hpp"

/**
 * @class SpotLightGPU
 * @brief Represents a spot light in GPU memory.
 *
 * This class is designed to hold the properties of a spot light,
 * such as its position, direction, inner and outer cutoffs, and color,
 * for efficient access in GPU rendering.
 */
class SpotLightGPU {
private:
  const SpotLight* m_light = nullptr;

  linalg::Vec3f m_position         = linalg::Vec3f(0.0F);
  linalg::Vec3f m_direction        = linalg::Vec3f(0.0F, 0.0F, -1.0F);
  float         m_cos_inner_cutoff = 0.0F;
  float         m_cos_outer_cutoff = 0.0F;
  linalg::Vec3f m_color            = linalg::Vec3f(1.0F);

protected:
  const SpotLight* light() const { return m_light; }

public:
  /**
   * @brief Constructor for SpotLightGPU.
   * @param light Pointer to the SpotLight object to be represented in GPU memory.
   */
  explicit SpotLightGPU(const SpotLight* light);

  SpotLightGPU(const SpotLightGPU&)            = delete;
  SpotLightGPU& operator=(const SpotLightGPU&) = delete;
  SpotLightGPU(SpotLightGPU&&)                 = delete;
  SpotLightGPU& operator=(SpotLightGPU&&)      = delete;

  /**
   * @brief Retrieves the position, direction, inner and outer cutoffs, and color of the light.
   */
  void retrieveData();

  /**
   * @brief Gets the position of the spot light.
   * @return The position vector of the light.
   */
  linalg::Vec3f getPosition() const { return m_position; }

  /**
   * @brief Gets the direction of the spot light.
   * @return The direction vector of the light.
   */
  linalg::Vec3f getDirection() const { return m_direction; }

  /**
   * @brief Gets the cosine of the inner cutoff angle of the spot light.
   * @return The cosine of the inner cutoff angle.
   */
  float getCosInnerCutoff() const { return m_cos_inner_cutoff; }

  /**
   * @brief Gets the cosine of the outer cutoff angle of the spot light.
   * @return The cosine of the outer cutoff angle.
   */
  float getCosOuterCutoff() const { return m_cos_outer_cutoff; }

  /**
   * @brief Gets the color of the spot light multiplied by its intensity.
   * @return The color vector of the light multiplied by the intensity.
   */
  linalg::Vec3f getColor() const { return m_color; }

  ~SpotLightGPU() = default; ///< Default destructor for SpotLightGPU.
};

#endif // GPU_LIGHTS_SPOTGPU_HPP