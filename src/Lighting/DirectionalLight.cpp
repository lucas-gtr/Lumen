#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Lighting/DirectionalLight.hpp"

void DirectionalLight::setDirection(const lin::Vec3d& direction) {
  m_direction = direction.normalized();
  getLightChangedObserver().notify();
}

lin::Vec3d DirectionalLight::getDirection() const { return m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters)
lin::Vec3d DirectionalLight::getDirectionFromPoint(const lin::Vec3d& point) const { return -m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters)
ColorRGB DirectionalLight::getLightFactor(const lin::Vec3d& point, const lin::Vec3d& normal) const {
  const double dot_product = lin::dot(-m_direction.normalized(), normal.normalized());
  return getIntensity() * getColor() * std::max(0.0, dot_product);
}