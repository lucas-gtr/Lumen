/**
 * @file PointLight.hpp
 * @brief Header file for the PointLight class.
 */
#ifndef LIGHTING_POINTLIGHT_HPP
#define LIGHTING_POINTLIGHT_HPP

#include <linalg/Vec3.hpp>

#include "Core/Color.hpp"
#include "Lighting/Light.hpp"

/**
 * @class PointLight
 * @brief A class representing a point light source in a 3D scene.
 *
 * This class inherits from the Light class and provides functionality specific to point lights,
 * including calculating the direction and light factor from a given point.
 * Point lights emit light in all directions from a single point in space.
 */
class PointLight : public Light {
public:
  PointLight() : Light(LightType::POINT) {} ///< Constructor with light type.

  PointLight(const PointLight&)            = delete;
  PointLight& operator=(const PointLight&) = delete;
  PointLight(PointLight&&)                 = delete;
  PointLight& operator=(PointLight&&)      = delete;

  /**
   * @brief Gets the direction of the light from a given point.
   * @param point The point from which to calculate the direction.
   * @return The direction vector from the point to the light.
   */
  linalg::Vec3d getDirectionFromPoint(const linalg::Vec3d& point) const override;

  /**
   * @brief Gets the light factor at a given point and normal.
   * @param point The point at which to calculate the light factor.
   * @param normal The normal vector at the point.
   * @return The light factor at the given point and normal.
   */
  ColorRGB getLightFactor(const linalg::Vec3d& point, const linalg::Vec3d& normal) const override;

  ~PointLight() override = default; ///< Default destructor.
};

#endif // LIGHTING_POINTLIGHT_HPP