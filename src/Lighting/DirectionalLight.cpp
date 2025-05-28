#include <Eigen/Core>
#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Lighting/DirectionalLight.hpp"

void DirectionalLight::setDirection(const Eigen::Vector3d& direction) { m_direction = direction.normalized(); }

Eigen::Vector3d DirectionalLight::getDirection() const { return m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters)
Eigen::Vector3d DirectionalLight::getDirectionFromPoint(const Eigen::Vector3d& point) const { return -m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters, bugprone-easily-swappable-parameters)
ColorRGB DirectionalLight::getLightFactor(const Eigen::Vector3d& point, const Eigen::Vector3d& normal) const {
  const double dot_product = normal.dot(-m_direction);
  return getIntensity() * getColor() * std::max(0.0, dot_product);
}