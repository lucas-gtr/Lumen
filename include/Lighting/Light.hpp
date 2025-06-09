/**
 * @file Light.hpp
 * @brief Header file for the Light class.
 */
#ifndef LIGHTING_LIGHT_HPP
#define LIGHTING_LIGHT_HPP

#include <cstdint>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Transform.hpp"

enum class LightType : std::uint8_t { Point, Directional, Spot };

/**
 * @class Light
 * @brief A class representing a light source in a 3D scene.
 *
 * This class inherits from the Transform class and provides functionality
 * for setting and getting the color and intensity of the light.
 * It can be used to represent different types of lights, such as point lights,
 * directional lights, and spotlights.
 */
class Light : public Transform {
private:
  LightType m_type;

  ColorRGB m_color     = {1.0, 1.0, 1.0};
  double   m_intensity = 1.0;

public:
  explicit Light(LightType light_type) : m_type(light_type) {} ///< Constructor with light type.

  Light(const Light&)            = default; ///< Default copy constructor.
  Light& operator=(const Light&) = default; ///< Default copy assignment operator.
  Light(Light&&)                 = default; ///< Default move constructor.
  Light& operator=(Light&&)      = default; ///< Default move assignment operator.

  void setColor(const ColorRGB& color) { m_color = color; }
  void setIntensity(double intensity) { m_intensity = intensity; }

  ColorRGB  getColor() const { return m_color; }
  double    getIntensity() const { return m_intensity; }
  LightType getType() const { return m_type; }

  /**
   * @brief Gets the direction of the light from a given point.
   * @param point The point from which to calculate the direction.
   * @return The direction vector from the point to the light.
   */
  virtual lin::Vec3d getDirectionFromPoint(const lin::Vec3d& point) const = 0;

  /**
   * @brief Gets the light factor at a given point and normal.
   * @param point The point at which to calculate the light factor.
   * @param normal The normal vector at the point.
   * @return The light factor at the given point and normal.
   */
  virtual ColorRGB getLightFactor(const lin::Vec3d& point, const lin::Vec3d& normal) const = 0;

  virtual ~Light() = default; ///< Default destructor.
};

#endif // LIGHTING_LIGHT_HPP