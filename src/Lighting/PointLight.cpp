#include <Eigen/Core>
#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Lighting/PointLight.hpp"

lin::Vec3 PointLight::getDirectionFromPoint(const lin::Vec3& point) const {
  return (getPosition() - point).normalized();
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ColorRGB PointLight::getLightFactor(const lin::Vec3& point, const lin::Vec3& normal) const {
  const double    distance    = (point - getPosition()).length();
  const double    attenuation = getIntensity() / (distance * distance);
  const lin::Vec3 light_dir   = (getPosition() - point).normalized();
  const double    dot_product = dot(normal, light_dir);
  return attenuation * getColor() * std::max(0.0, dot_product);
}