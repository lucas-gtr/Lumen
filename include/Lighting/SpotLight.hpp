/**
 * @file SpotLight.hpp
 * @brief Header file for the SpotLight class.
 */
#ifndef LIGHTING_SPOTLIGHT_HPP
#define LIGHTING_SPOTLIGHT_HPP

#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Core/Math/Vec3.hpp"
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
  lin::Vec3 m_direction   = {0.0, 0.0, -1.0};
  double    m_inner_angle = DEFAULT_SPOT_LIGHT_INNER_ANGLE;
  double    m_outer_angle = DEFAULT_SPOT_LIGHT_OUTER_ANGLE;

public:
  SpotLight() : Light(LightType::Spot) {}           ///< Constructor with light type.
  SpotLight(const SpotLight&)            = default; ///< Default copy constructor.
  SpotLight& operator=(const SpotLight&) = default; ///< Default copy assignment operator.
  SpotLight(SpotLight&&)                 = default; ///< Default move constructor.
  SpotLight& operator=(SpotLight&&)      = default; ///< Default move assignment operator.

  /**
   * @brief Gets the direction of the spot light.
   * @return The direction vector of the light.
   */
  lin::Vec3 getDirection() const { return m_direction; }

  /**
   * @brief Sets the direction of the spot light.
   * @param direction The new direction vector of the light.
   */
  void setDirection(const lin::Vec3& direction) { m_direction = direction; }

  /**
   * @brief Gets the inner angle of the spot light.
   * @return The inner angle in degrees.
   */
  double getInnerAngle() const { return m_inner_angle * DEG_TO_RAD; }

  /**
   * @brief Sets the inner angle of the spot light.
   * @param inner_angle The new inner angle in degrees.
   */
  void setInnerAngle(double inner_angle) {
    m_inner_angle = std::clamp(inner_angle, MIN_SPOT_LIGHT_ANGLE, m_outer_angle);
  }

  /**
   * @brief Gets the outer angle of the spot light.
   * @return The outer in degrees.
   */
  double getOuterAngle() const { return m_outer_angle * DEG_TO_RAD; }

  /**
   * @brief Sets the outer angle of the spot light.
   * @param outer_angle The new outer angle in degrees.
   */
  void setOuterAngle(double outer_angle) {
    m_outer_angle = std::clamp(outer_angle, m_inner_angle, MAX_SPOT_LIGHT_ANGLE);
  }

  /**
   * @brief Gets the direction of the light from a given point.
   * @param point The point from which to calculate the direction.
   * @return The direction vector from the point to the light.
   */
  lin::Vec3 getDirectionFromPoint(const lin::Vec3& point) const override;

  /**
   * @brief Gets the light factor at a given point and normal.
   * @param point The point at which to calculate the light factor.
   * @param normal The normal vector at the point.
   * @return The light factor at the given point and normal.
   */
  ColorRGB getLightFactor(const lin::Vec3& point, const lin::Vec3& normal) const override;

  ~SpotLight() override = default; ///< Default destructor.
};

#endif // LIGHTING_SPOTLIGHT_HPP