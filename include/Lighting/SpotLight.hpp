/**
 * @file SpotLight.hpp
 * @brief Header file for the SpotLight class.
 */
#ifndef LIGHTING_SPOTLIGHT_HPP
#define LIGHTING_SPOTLIGHT_HPP

#include <algorithm>
#include <linalg/Vec3.hpp>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "Lighting/Light.hpp"

/**
 * @class SpotLight
 * @brief A class representing a spot light source in a 3D scene.
 *
 *
 */
class SpotLight : public Light {
private:
  linalg::Vec3d m_direction   = {0.0, 0.0, -1.0};
  double     m_inner_angle = DEFAULT_SPOT_LIGHT_INNER_ANGLE; // in degrees
  double     m_outer_angle = DEFAULT_SPOT_LIGHT_OUTER_ANGLE; // in degrees

public:
  SpotLight() : Light(LightType::SPOT) {} ///< Constructor with light type.

  SpotLight(const SpotLight&)            = delete;
  SpotLight& operator=(const SpotLight&) = delete;
  SpotLight(SpotLight&&)                 = delete;
  SpotLight& operator=(SpotLight&&)      = delete;

  /**
   * @brief Gets the direction of the spot light.
   * @return The direction vector of the light.
   */
  linalg::Vec3d getDirection() const { return m_direction; }

  /**
   * @brief Sets the direction of the spot light.
   * @param direction The new direction vector of the light.
   */
  void setDirection(const linalg::Vec3d& direction);

  /**
   * @brief Gets the inner angle of the spot light.
   * @return The inner angle in degrees.
   */
  double getInnerAngle() const { return m_inner_angle * DEG_TO_RAD; }

  /**
   * @brief Sets the inner angle of the spot light.
   * @param inner_angle The new inner angle in degrees.
   */
  void setInnerAngle(double inner_angle);

  /**
   * @brief Gets the outer angle of the spot light.
   * @return The outer in degrees.
   */
  double getOuterAngle() const { return m_outer_angle * DEG_TO_RAD; }

  /**
   * @brief Sets the outer angle of the spot light.
   * @param outer_angle The new outer angle in degrees.
   */
  void setOuterAngle(double outer_angle);

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

  ~SpotLight() override = default; ///< Default destructor.
};

#endif // LIGHTING_SPOTLIGHT_HPP