#include <algorithm>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "Core/CommonTypes.hpp"
#include "Lighting/DirectionalLight.hpp"

void DirectionalLight::setDirection(const linalg::Vec3d& direction) {
  m_direction = direction.normalized();
  getLightChangedObserver().notify();
}

linalg::Vec3d DirectionalLight::getDirection() const { return m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters)
linalg::Vec3d DirectionalLight::getDirectionFromPoint(const linalg::Vec3d& point) const { return -m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters)
ColorRGB DirectionalLight::getLightFactor(const linalg::Vec3d& point, const linalg::Vec3d& normal) const {
  const double dot_product = linalg::dot(-m_direction.normalized(), normal.normalized());
  return getIntensity() * getColor() * std::max(0.0, dot_product);
}