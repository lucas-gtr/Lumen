#include <Eigen/Core>
#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Lighting/DirectionalLight.hpp"

void DirectionalLight::setDirection(const lin::Vec3& direction) { m_direction = direction.normalized(); }

lin::Vec3 DirectionalLight::getDirection() const { return m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters)
lin::Vec3 DirectionalLight::getDirectionFromPoint(const lin::Vec3& point) const { return -m_direction; }

// NOLINTNEXTLINE(misc-unused-parameters, bugprone-easily-swappable-parameters)
ColorRGB DirectionalLight::getLightFactor(const lin::Vec3& point, const lin::Vec3& normal) const {
  const double dot_product = lin::dot(-m_direction.normalized(), normal.normalized());
  return getIntensity() * getColor() * std::max(0.0, dot_product);
}