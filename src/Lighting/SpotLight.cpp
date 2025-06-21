#include <algorithm>
#include <cmath>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Core/MathConstants.hpp"
#include "Lighting/SpotLight.hpp"

void SpotLight::setDirection(const lin::Vec3d& direction) {
  m_direction = direction.normalized();
  getLightChangedObserver().notify();
}

void SpotLight::setInnerAngle(double inner_angle) {
  m_inner_angle = std::clamp(inner_angle, MIN_SPOT_LIGHT_ANGLE, m_outer_angle);
  getLightChangedObserver().notify();
}

void SpotLight::setOuterAngle(double outer_angle) {
  m_outer_angle = std::clamp(outer_angle, m_inner_angle, MAX_SPOT_LIGHT_ANGLE);
  getLightChangedObserver().notify();
}

lin::Vec3d SpotLight::getDirectionFromPoint(const lin::Vec3d& point) const {
  return (getPosition() - point).normalized();
}

ColorRGB SpotLight::getLightFactor(const lin::Vec3d& point, const lin::Vec3d& normal) const {
  const double distance    = (getPosition() - point).length();
  const double attenuation = getIntensity() / (distance * distance);

  const lin::Vec3d light_dir   = (getPosition() - point).normalized();
  const double     dot_product = dot(normal, light_dir);

  const double cosine_angle = lin::dot(-light_dir, m_direction.normalized());
  const double inner_cutoff = std::cos(m_inner_angle * DEG_TO_RAD);
  const double outer_cutoff = std::cos(m_outer_angle * DEG_TO_RAD);

  const double spot_factor = std::clamp((cosine_angle - outer_cutoff) / (inner_cutoff - outer_cutoff), 0.0, 1.0);

  return attenuation * getColor() * std::max(0.0, dot_product) * spot_factor;
}