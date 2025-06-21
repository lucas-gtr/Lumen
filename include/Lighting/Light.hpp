/**
 * @file Light.hpp
 * @brief Header file for the Light class.
 */
#ifndef LIGHTING_LIGHT_HPP
#define LIGHTING_LIGHT_HPP

#include <cstdint>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Observer.hpp"
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

  Observer<> m_light_changed_observer;

public:
  /**
   * @brief Default constructor that initializes the light with a point light type.
   * @param light_type The type of the light.
   */
  explicit Light(LightType light_type) : m_type(light_type) {}

  Light(const Light&)            = delete;
  Light& operator=(const Light&) = delete;
  Light(Light&&)                 = delete;
  Light& operator=(Light&&)      = delete;

  /**
   * @brief Gets the observer for light changes.
   * @return A reference to the observer that notifies when the light properties change.
   */
  Observer<>& getLightChangedObserver() { return m_light_changed_observer; }

  /**
   * @brief Sets the color of the light.
   * @param color The color of the light.
   */
  void setColor(const ColorRGB& color);

  /**
   * @brief Sets the intensity of the light.
   * @param intensity The intensity of the light.
   */
  void setIntensity(double intensity);

  /**
   * @brief Gets the color and intensity of the light.
   * @return A pair containing the color and intensity of the light.
   */
  ColorRGB getColor() const { return m_color; }

  /**
   * @brief Gets the intensity of the light.
   * @return The intensity of the light.
   */
  double getIntensity() const { return m_intensity; }

  /**
   * @brief Gets the type of the light.
   * @return The type of the light as a LightType enum.
   */
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