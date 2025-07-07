#include <algorithm>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "Core/CommonTypes.hpp"
#include "Lighting/PointLight.hpp"

linalg::Vec3d PointLight::getDirectionFromPoint(const linalg::Vec3d& point) const {
  return (getPosition() - point).normalized();
}

ColorRGB PointLight::getLightFactor(const linalg::Vec3d& point, const linalg::Vec3d& normal) const {
  const double     distance    = (point - getPosition()).length();
  const double     attenuation = getIntensity() / (distance * distance);
  const linalg::Vec3d light_dir   = (getPosition() - point).normalized();
  const double     dot_product = dot(normal, light_dir);
  return attenuation * getColor() * std::max(0.0, dot_product);
}