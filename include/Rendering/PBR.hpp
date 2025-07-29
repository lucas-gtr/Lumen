/**
 * @file PBR.hpp
 * @brief Header file for the PBR (Physically Based Rendering) utilities.
 */
#ifndef RENDERING_PBR_HPP
#define RENDERING_PBR_HPP

#include <algorithm>
#include <linalg/Mat3.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "Core/CommonTypes.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Diagnostics/ScopedTimer.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

namespace PBR {

static constexpr double BASE_REFLECTANCE = 0.04; ///< Base reflectance value for non-metallic surfaces.

struct SurfaceInteraction {
  linalg::Vec3d normal;
  linalg::Vec3d tangent;
  linalg::Vec3d bitangent;
  linalg::Vec3d incoming_direction;

  ColorRGB base_color;
  double   roughness{};
  double   metalness{};
};

inline linalg::Vec3d reflect(const linalg::Vec3d& incident, const linalg::Vec3d& normal) {
  return incident - 2 * linalg::dot(incident, normal) * normal;
}

inline linalg::Vec3d sampleCosineHemisphere() {
  const double r1 = randomUniform01();
  const double r2 = randomUniform01();

  const double r     = std::sqrt(r1);
  const double theta = 2.0 * M_PI * r2;

  const double x = r * std::cos(theta);
  const double y = r * std::sin(theta);
  const double z = std::sqrt(1.0 - r1);

  return {x, y, z};
}

inline ColorRGB getBaseReflectance(const ColorRGB& albedo, double metalness) {
  return ColorRGB(BASE_REFLECTANCE) * (1.0 - metalness) + albedo * metalness;
}

// NOLINTBEGIN(readability-identifier-naming)
inline ColorRGB getFresnelSchlick(const linalg::Vec3d& half_vector, const linalg::Vec3d& view_direction,
                                  const ColorRGB& F0) {
  const double cos_theta = std::max(0.0, linalg::dot(view_direction, half_vector));
  return F0 + (ColorRGB(1.0) - F0) * std::pow(1.0 - cos_theta, 5); // NOLINT
}

inline double getDistributionGgx(const linalg::Vec3d& normal, const linalg::Vec3d& half_vector, double roughness) {
  const double alpha      = std::max(0.001, roughness * roughness);
  const double cos_theta  = std::max(0.0, linalg::dot(normal, half_vector));
  const double cos_theta2 = cos_theta * cos_theta;

  const double denom = (cos_theta2 * (alpha * alpha - 1.0) + 1.0);
  return (alpha * alpha) / (PI * denom * denom);
}
// NOLINTEND(readability-identifier-naming)

inline linalg::Vec3d sampleHalfVectorGgx(double roughness) {
  const double alpha = roughness * roughness;
  const double u1    = randomUniform01();
  const double u2    = randomUniform01();

  const double theta = std::atan(alpha * std::sqrt(u1 / (1.0 - u1)));
  const double phi   = 2.0 * M_PI * u2;

  const double sin_theta = std::sin(theta);

  return {sin_theta * std::cos(phi), sin_theta * std::sin(phi), std::cos(theta)};
}

inline void convertToWorldSpace(linalg::Vec3d& local_vector, const linalg::Vec3d& normal, const linalg::Vec3d& tangent,
                                const linalg::Vec3d& bitangent) {
  local_vector = (local_vector.x * tangent + local_vector.y * bitangent + local_vector.z * normal).normalized();
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

inline ColorRGB getLambertianDiffuse(const ColorRGB& diffuse_color) { return diffuse_color * INV_PI; }

// NOLINTBEGIN(readability-identifier-naming)
inline double getCookTorranceSpecular(const linalg::Vec3d& incoming_direction, const linalg::Vec3d& outgoing_direction,
                                      const linalg::Vec3d& normal, double roughness) {
  const linalg::Vec3d half_vector = (incoming_direction + outgoing_direction).normalized();
  const double        D           = getDistributionGgx(normal, half_vector, roughness);
  const double        G           = getGeometrySmith(normal, incoming_direction, outgoing_direction, roughness);

  const double cos_theta_l = std::max(0.0, linalg::dot(normal, outgoing_direction));
  if(cos_theta_l <= 0.0) {
    return 0.0;
  }

  const double cos_theta_v = std::max(0.0, linalg::dot(normal, incoming_direction));
  if(cos_theta_v <= 0.0) {
    return 0.0;
  }

  const double denominator =
      4.0 * cos_theta_l * cos_theta_v; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  return (D * G) / denominator;
}

inline linalg::Vec3d getOutgoingDirection(const SurfaceInteraction& interaction, double& pdf) {
  const linalg::Vec3d& normal    = interaction.normal;
  const linalg::Vec3d& tangent   = interaction.tangent;
  const linalg::Vec3d& bitangent = interaction.bitangent;

  const ColorRGB&      base_color         = interaction.base_color;
  const double         roughness          = interaction.roughness;
  const double         metalness          = interaction.metalness;
  const linalg::Vec3d& incoming_direction = interaction.incoming_direction;

  const ColorRGB F0                     = getBaseReflectance(ColorRGB(base_color), metalness);
  const ColorRGB F                      = getFresnelSchlick(normal, incoming_direction, F0);
  const double   reflection_probability = metalness + (1.0 - metalness) * ((F.r + F.g + F.b) / 3);

  const bool    reflect_ray = randomUniform01() < reflection_probability;
  linalg::Vec3d outgoing_direction;
  if(reflect_ray) {
    linalg::Vec3d half_vector = sampleHalfVectorGgx(roughness);
    convertToWorldSpace(half_vector, normal, tangent, bitangent);
    outgoing_direction = reflect(-incoming_direction, half_vector);
  } else {
    outgoing_direction = sampleCosineHemisphere();
    convertToWorldSpace(outgoing_direction, normal, tangent, bitangent);
  }

  const linalg::Vec3d half_vector = (incoming_direction + outgoing_direction).normalized();

  const double pdf_diffuse =
      (1.0 - reflection_probability) * std::max(0.0, linalg::dot(outgoing_direction, normal)) / PI;

  const double pdf_specular = reflection_probability * getDistributionGgx(normal, half_vector, roughness) /
                              (4.0 * linalg::dot(incoming_direction, normal));

  pdf = pdf_diffuse + pdf_specular;

  return outgoing_direction;
}

inline ColorRGB evaluateBrdf(const linalg::Vec3d& incoming_direction, const linalg::Vec3d& outgoing_direction,
                             const linalg::Vec3d& normal, const ColorRGB& base_color, double roughness,
                             double metalness) {
  const linalg::Vec3d& half_vector = (incoming_direction + outgoing_direction).normalized();

  const ColorRGB       F0          = getBaseReflectance(base_color, metalness);

  const ColorRGB k_s = getFresnelSchlick(half_vector, incoming_direction, F0);
  const ColorRGB k_d = (ColorRGB(1.0) - k_s) * (1.0 - metalness);

  const ColorRGB specular = getCookTorranceSpecular(incoming_direction, outgoing_direction, normal, roughness) * k_s;
  const ColorRGB diffuse  = getLambertianDiffuse(base_color) * k_d;

  return diffuse + specular;
}
// NOLINTEND(readability-identifier-naming)

inline ColorRGB getCookTorranceBrdf(const SurfaceInteraction& interaction, linalg::Vec3d& outgoing_direction) {
  double pdf         = 1.0;
  outgoing_direction = getOutgoingDirection(interaction, pdf);

  const linalg::Vec3d& normal             = interaction.normal;
  const double         attenuation_factor = linalg::dot(normal, outgoing_direction);

  if(attenuation_factor <= 0.0) {
    return ColorRGB(0.0);
  }

  const ColorRGB&      base_color         = interaction.base_color;
  const linalg::Vec3d& incoming_direction = interaction.incoming_direction;
  const double         metalness          = interaction.metalness;
  const double         roughness          = interaction.roughness;

  const ColorRGB brdf = evaluateBrdf(incoming_direction, outgoing_direction, normal, base_color, roughness, metalness) *
                        attenuation_factor;
  return brdf / pdf;
}
}; // namespace PBR

#endif // RENDERING_PBR_HPP