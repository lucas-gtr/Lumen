#include <Eigen/Core>
#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Lighting/PointLight.hpp"

Eigen::Vector3d PointLight::getDirectionFromPoint(const Eigen::Vector3d& point) const {
  return (getPosition() - point).normalized();
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ColorRGB PointLight::getLightFactor(const Eigen::Vector3d& point, const Eigen::Vector3d& normal) const {
  const double          distance    = (point - getPosition()).norm();
  const double          attenuation = getIntensity() / (distance * distance);
  const Eigen::Vector3d light_dir   = (getPosition() - point).normalized();
  const double          dot_product = normal.dot(light_dir);
  return attenuation * getColor() * std::max(0.0, dot_product);
}