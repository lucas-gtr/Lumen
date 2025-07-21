/**
 * @file LightGPU.hpp
 * @brief Header file for the LightGPU class, which represents a light source in GPU rendering
 */
#ifndef GPU_LIGHTS_LIGHTGPU_HPP
#define GPU_LIGHTS_LIGHTGPU_HPP

#include <linalg/Vec3.hpp>

#include "Lighting/Light.hpp"

/**
 * @class LightGPU
 * @brief A class that represents a light source in GPU rendering.
 *
 * This class is used to manage the properties of a light source in GPU rendering,
 * including its color and intensity. It provides methods to retrieve the light data
 * and to get the color of the light multiplied by its intensity.
 */
class LightGPU {
private:
  linalg::Vec3f m_color; ///< Color of the light multiplied by intensity.

protected:
  void setColor(const linalg::Vec3f& color) { m_color = color; }

public:
  /**
   * @brief Default constructor for LightGPU.
   */
  LightGPU() = default;

  LightGPU(const LightGPU&)            = delete;
  LightGPU& operator=(const LightGPU&) = delete;
  LightGPU(LightGPU&&)                 = delete;
  LightGPU& operator=(LightGPU&&)      = delete;

  /**
   * @brief Retrieves the data of the light.
   */
  virtual void retrieveData() = 0;

  /**
   * @brief Gets the light source.
   * @return A pointer to the Light source.
   */
  virtual const Light* getSource() const = 0;

  /**
   * @brief Gets the color of the light multiplied by the intensity.
   * @return The color vector of the light multiplied by the intensity.
   */
  virtual linalg::Vec3f getColor() const { return m_color; }

  /**
   * @brief Default destructor for LightGPU.
   */
  virtual ~LightGPU() = default;
};

#endif // GPU_LIGHTS_LIGHTGPU_HPP