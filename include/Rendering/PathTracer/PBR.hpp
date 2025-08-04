/**
 * @file PBR.hpp
 * @brief Header file for the PBR (Physically Based Rendering) utilities.
 */
#ifndef RENDERING_PATHTRACER_PBR_HPP
#define RENDERING_PATHTRACER_PBR_HPP

#include <algorithm>
#include <linalg/Mat3.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "Core/Color.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Scene/LightSample.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

namespace PBR {

static constexpr double DIELECTRIC_REFLECTANCE = 0.04; ///< Base reflectance value for non-metallic surfaces.

inline ColorRGB getLambertianDiffuse(const ColorRGB& diffuse_color) { return diffuse_color * INV_PI; }

inline ColorRGB getBaseReflectance(const ColorRGB& albedo, double metalness) {
  return ColorRGB(DIELECTRIC_REFLECTANCE) * (1.0 - metalness) + albedo * metalness;
}

// NOLINTBEGIN(readability-identifier-naming)
inline ColorRGB getFresnelSchlick(const linalg::Vec3d& half_vector, const linalg::Vec3d& view_direction,
                                  const ColorRGB& F0) {
  const double cos_theta = std::max(0.0, linalg::dot(view_direction, half_vector));
  return F0 + (ColorRGB(1.0) - F0) * std::pow(1.0 - cos_theta, 5); // NOLINT
}

inline double getDistributionGgx(const linalg::Vec3d& normal, const linalg::Vec3d& half_vector, double roughness) {
  const double alpha      = std::max(1e-6, roughness * roughness);
  const double cos_theta  = std::max(0.0, linalg::dot(normal, half_vector));
  const double cos_theta2 = cos_theta * cos_theta;

  const double denom = (cos_theta2 * (alpha * alpha - 1.0) + 1.0);
  return (alpha * alpha) / (PI * denom * denom);
}

inline double getGeometrySmith(const linalg::Vec3d& normal, const linalg::Vec3d& view_dir,
                               const linalg::Vec3d& light_dir, double roughness) {
  const double alpha  = roughness * roughness;
  const double alpha2 = alpha * alpha;

  const double n_dot_v = std::max(0.0, linalg::dot(normal, view_dir));
  const double n_dot_l = std::max(0.0, linalg::dot(normal, light_dir));

  const double ggx_v = n_dot_v / (n_dot_v * (1.0 - alpha2) + alpha2);
  const double ggx_l = n_dot_l / (n_dot_l * (1.0 - alpha2) + alpha2);

  return ggx_v * ggx_l;
}

inline double getCookTorranceSpecular(const linalg::Vec3d& incoming_dir, const linalg::Vec3d& outgoing_direction,
                                      const linalg::Vec3d& normal, double roughness) {
  const linalg::Vec3d half_vector = (incoming_dir + outgoing_direction).normalized();
  const double        D           = getDistributionGgx(normal, half_vector, roughness);
  const double        G           = getGeometrySmith(normal, incoming_dir, outgoing_direction, roughness);

  const double cos_theta_l = std::max(0.0, linalg::dot(normal, outgoing_direction));
  if(cos_theta_l <= 0.0) {
    return 0.0;
  }

  const double cos_theta_v = std::max(0.0, linalg::dot(normal, incoming_dir));
  if(cos_theta_v <= 0.0) {
    return 0.0;
  }

  const double denominator =
      4.0 * cos_theta_l * cos_theta_v; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  return (D * G) / denominator;
}

struct BRDFInput {
  linalg::Vec3d incoming_dir;
  linalg::Vec3d normal;

  ColorRGB base_color;
  double   roughness{};
  double   metalness{};

  double specular_ratio{};

  BRDFInput(const linalg::Vec3d& incoming_dir, const linalg::Vec3d& normal, const ColorRGB& base_color,
            double roughness, double metalness)
      : incoming_dir(incoming_dir), normal(normal), base_color(base_color), roughness(roughness), metalness(metalness) {

    const ColorRGB F0 = getBaseReflectance(base_color, metalness);
    const ColorRGB F  = getFresnelSchlick(normal, incoming_dir, F0);

    specular_ratio = metalness + (1.0 - metalness) * F.luminance();
  }
};

inline ColorRGB evaluateBrdf(const BRDFInput& input, const linalg::Vec3d& outgoing_direction) {
  const linalg::Vec3d& incoming_dir = input.incoming_dir;
  const linalg::Vec3d& half_vector  = (incoming_dir + outgoing_direction).normalized();

  const ColorRGB F0 = getBaseReflectance(input.base_color, input.metalness);

  const ColorRGB k_s = getFresnelSchlick(half_vector, incoming_dir, F0);
  const ColorRGB k_d = (ColorRGB(1.0) - k_s) * (1.0 - input.metalness);

  const double   specular = getCookTorranceSpecular(incoming_dir, outgoing_direction, input.normal, input.roughness);
  const ColorRGB diffuse  = getLambertianDiffuse(input.base_color);

  return k_d * diffuse + k_s * specular;
}
// NOLINTEND(readability-identifier-naming)

}; // namespace PBR

#endif // RENDERING_PATHTRACER_PBR_HPP