#include <algorithm>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Lighting/PointLight.hpp"

lin::Vec3d PointLight::getDirectionFromPoint(const lin::Vec3d& point) const {
  return (getPosition() - point).normalized();
}

ColorRGB PointLight::getLightFactor(const lin::Vec3d& point, const lin::Vec3d& normal) const {
  const double     distance    = (point - getPosition()).length();
  const double     attenuation = getIntensity() / (distance * distance);
  const lin::Vec3d light_dir   = (getPosition() - point).normalized();
  const double     dot_product = dot(normal, light_dir);
  return attenuation * getColor() * std::max(0.0, dot_product);
}