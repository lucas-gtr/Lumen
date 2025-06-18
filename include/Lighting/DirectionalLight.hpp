/**
 * @file DirectionalLight.hpp
 * @brief Header file for the DirectionalLight class.
 */
#ifndef LIGHTING_DIRECTIONALLIGHT_HPP
#define LIGHTING_DIRECTIONALLIGHT_HPP

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Lighting/Light.hpp"

/**
 * @class DirectionalLight
 * @brief A class representing a directional light source in a 3D scene.
 *
 * This class inherits from the Light class and provides functionality specific to directional lights,
 * including setting and getting the direction of the light.
 */
class DirectionalLight : public Light {
private:
  lin::Vec3d m_direction = {0.0, 0.0, -1.0};

public:
  DirectionalLight() : Light(LightType::Directional) {} ///< Constructor with light type.

  DirectionalLight(const DirectionalLight&)            = delete;
  DirectionalLight& operator=(const DirectionalLight&) = delete;
  DirectionalLight(DirectionalLight&&)                 = delete;
  DirectionalLight& operator=(DirectionalLight&&)      = delete;

  /**
   * @brief Gets the direction of the directional light.
   * @return The direction vector of the light.
   */
  lin::Vec3d getDirection() const;

  /**
   * @brief Sets the direction of the directional light.
   * @param direction The new direction vector of the light.
   */
  void setDirection(const lin::Vec3d& direction);

  /**
   * @brief Gets the direction of the light from a given point.
   * @param point The point from which to calculate the direction.
   * @return The direction vector from the point to the light.
   * This function always returns the same direction vector,
   * as the light is directional and does not depend on the point.
   */
  lin::Vec3d getDirectionFromPoint(const lin::Vec3d& point) const override;

  /**
   * @brief Gets the light factor at a given point and normal.
   * @param point The point at which to calculate the light factor.
   * @param normal The normal vector at the point.
   * @return The light factor at the given point and normal.
   */
  ColorRGB getLightFactor(const lin::Vec3d& point, const lin::Vec3d& normal) const override;

  ~DirectionalLight() override = default; ///< Default destructor.
};

#endif // LIGHTING_DIRECTIONALLIGHT_HPP