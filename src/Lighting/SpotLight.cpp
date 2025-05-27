#include <Eigen/Core>
#include <algorithm>
#include <cmath>

#include "Core/MathConstants.hpp"
#include "Lighting/SpotLight.hpp"

Eigen::Vector3d SpotLight::getDirectionFromPoint(const Eigen::Vector3d& point) const {
  return (getPosition() - point).normalized();
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ColorRGB SpotLight::getLightFactor(const Eigen::Vector3d& point, const Eigen::Vector3d& normal) const {
  const double distance    = (getPosition() - point).norm();
  const double attenuation = getIntensity() / (distance * distance);

  const Eigen::Vector3d light_dir   = (getPosition() - point).normalized();
  const double          dot_product = normal.dot(light_dir);

  const double cosine_angle = getDirection().dot(-light_dir);
  const double inner_cutoff = std::cos(m_inner_angle * DEG_TO_RAD);
  const double outer_cutoff = std::cos(m_outer_angle * DEG_TO_RAD);

  const double spot_factor = std::clamp((cosine_angle - outer_cutoff) / (inner_cutoff - outer_cutoff), 0.0, 1.0);

  return attenuation * getColor() * std::max(0.0, dot_product) * spot_factor;
}