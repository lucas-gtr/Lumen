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
#include <utility>

#include "Core/Color.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Scene/LightSample.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

namespace PBR {

static constexpr double AIR_IOR = 1.00029;

struct BSDFInput {
  linalg::Vec3d incoming_dir;
  linalg::Vec3d normal;

  ColorRGB base_color;
  double   roughness{};
  double   metalness{};
  double   transmission{};
  double   ior_from{};
  double   ior_to{};

  double critical_angle_cosine{};

  double distance{};

  BSDFInput(const linalg::Vec3d& incoming_dir, const linalg::Vec3d& normal, const ColorRGB& base_color,
            double roughness, double metalness, double transmission, double ior_material, double distance)
      : incoming_dir(incoming_dir), normal(normal), base_color(base_color), roughness(roughness), metalness(metalness),
        transmission(transmission), ior_from(AIR_IOR), ior_to(ior_material), distance(distance) {

    if(linalg::dot(normal, incoming_dir) < 0.0) {
      this->normal = -normal;
      std::swap(this->ior_from, this->ior_to);
    }

    if(ior_from > ior_to) {
      critical_angle_cosine = std::sqrt(1.0 - (ior_to * ior_to) / (ior_from * ior_from));
    }
  }
};

inline ColorRGB getBaseReflectance(const ColorRGB& albedo, double metalness, double ior_from, double ior_to) {
  const double r0 = ((ior_from - ior_to) * (ior_from - ior_to)) / ((ior_from + ior_to) * (ior_from + ior_to));
  return ColorRGB(r0) * (1.0 - metalness) + albedo * metalness;
}

// NOLINTBEGIN(readability-identifier-naming)
inline ColorRGB getFresnelSchlick(const linalg::Vec3d& half_vector, const linalg::Vec3d& view_direction,
                                  const ColorRGB& F0, double critical_angle_cosine) {
  const double cos_theta = std::max(0.0, linalg::dot(view_direction, half_vector));
  if(cos_theta < critical_angle_cosine) {
    return ColorRGB(1.0); // Total internal reflection
  }
  return F0 +
         (ColorRGB(1.0) - F0) *
             std::pow(1.0 - cos_theta, 5); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}

struct LobeProbabilities {
  double diffuse{};
  double specular{};
  double transmission{};

  // NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)
  LobeProbabilities(const BSDFInput& bsdf_input, const linalg::Vec3d& micro_normal) {
    const ColorRGB F0 =
        getBaseReflectance(bsdf_input.base_color, bsdf_input.metalness, bsdf_input.ior_from, bsdf_input.ior_to);
    const ColorRGB F = getFresnelSchlick(micro_normal, bsdf_input.incoming_dir, F0, bsdf_input.critical_angle_cosine);

    specular     = bsdf_input.metalness + (1.0 - bsdf_input.metalness) * F.luminance();
    transmission = (1.0 - specular) * bsdf_input.transmission;
    diffuse      = 1.0 - specular - transmission;
  }
  // NOLINTEND(cppcoreguidelines-prefer-member-initializer)
};

inline ColorRGB getLambertianDiffuse(const ColorRGB& diffuse_color) { return diffuse_color * INV_PI; }

inline double getDistributionGgx(const linalg::Vec3d& normal, const linalg::Vec3d& half_vector, double roughness) {
  const double alpha      = std::max(1e-6, roughness * roughness);
  const double alpha2     = alpha * alpha;
  const double cos_theta  = std::max(0.0, linalg::dot(normal, half_vector));
  const double cos_theta2 = cos_theta * cos_theta;

  const double denom = (cos_theta2 * (alpha2 - 1.0) + 1.0);
  return (alpha2) / (PI * denom * denom);
}

inline double getGeometrySmith(const linalg::Vec3d& normal, const linalg::Vec3d& view_dir,
                               const linalg::Vec3d& light_dir, double roughness) {
  const double alpha  = roughness * roughness;
  const double alpha2 = alpha * alpha;

  const double n_dot_v = std::abs(linalg::dot(normal, view_dir));
  const double n_dot_l = std::abs(linalg::dot(normal, light_dir));

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

inline ColorRGB evaluateBrdf(const BSDFInput& input, const linalg::Vec3d& outgoing_direction) {
  const linalg::Vec3d& incoming_dir = input.incoming_dir;
  const linalg::Vec3d& half_vector  = (incoming_dir + outgoing_direction).normalized();

  const ColorRGB F0 = getBaseReflectance(input.base_color, input.metalness, input.ior_from, input.ior_to);

  const ColorRGB k_s = getFresnelSchlick(half_vector, incoming_dir, F0, input.critical_angle_cosine);
  const ColorRGB k_d = (ColorRGB(1.0) - k_s) * (1.0 - input.metalness) * (1.0 - input.transmission);

  const double   specular = getCookTorranceSpecular(incoming_dir, outgoing_direction, input.normal, input.roughness);
  const ColorRGB diffuse  = getLambertianDiffuse(input.base_color);

  ColorRGB brdf = k_d * diffuse + k_s * specular;

  if(input.ior_from > input.ior_to) {
    ColorRGB light_attenuation(1.0);
    light_attenuation.r = std::exp(-(1.0 - input.base_color.r) * input.distance);
    light_attenuation.g = std::exp(-(1.0 - input.base_color.g) * input.distance);
    light_attenuation.b = std::exp(-(1.0 - input.base_color.b) * input.distance);
    brdf *= light_attenuation;
  }

  return brdf;
}

inline ColorRGB evaluateBtdf(const BSDFInput& input, const linalg::Vec3d& outgoing_direction) {
  const double ior_from = input.ior_from;
  const double ior_to   = input.ior_to;

  linalg::Vec3d half_vector = (input.incoming_dir * ior_from + outgoing_direction * ior_to).normalized();
  ColorRGB      light_attenuation(1.0);
  if(ior_from < ior_to) {
    half_vector = -half_vector;
  } else {
    light_attenuation.r = std::exp(-(1.0 - input.base_color.r) * input.distance);
    light_attenuation.g = std::exp(-(1.0 - input.base_color.g) * input.distance);
    light_attenuation.b = std::exp(-(1.0 - input.base_color.b) * input.distance);
  }

  const double cos_theta_i_ht = std::max(1e-6, linalg::dot(input.incoming_dir, half_vector));
  const double cos_theta_o_ht = std::max(1e-6, linalg::dot(outgoing_direction, -half_vector));
  const double cos_theta_i_n  = std::max(0.0, linalg::dot(input.incoming_dir, input.normal));
  const double cos_theta_o_n  = std::max(0.0, linalg::dot(outgoing_direction, -input.normal));

  const double angle_factor = (cos_theta_i_ht * cos_theta_o_ht) / (cos_theta_i_n * cos_theta_o_n);
  const double denominator  = (ior_from * cos_theta_i_ht + ior_to * cos_theta_o_ht);

  const double D = getDistributionGgx(input.normal, half_vector, input.roughness);
  const double G = getGeometrySmith(input.normal, input.incoming_dir, outgoing_direction, input.roughness);

  const ColorRGB F0 = getBaseReflectance(input.base_color, input.metalness, input.ior_from, input.ior_to);
  const double   k_t =
      (ColorRGB(1.0) - getFresnelSchlick(input.normal, input.incoming_dir, F0, input.critical_angle_cosine))
          .luminance() *
      (1.0 - input.metalness) * input.transmission;

  return light_attenuation * ColorRGB(angle_factor * k_t * D * G * ior_to * ior_to / (denominator * denominator));
}

inline ColorRGB evaluateBsdf(const BSDFInput& input, const linalg::Vec3d& outgoing_direction) {
  if(linalg::dot(input.normal, outgoing_direction) > 0.0) {
    return evaluateBrdf(input, outgoing_direction);
  }
  return evaluateBtdf(input, outgoing_direction);
}
// NOLINTEND(readability-identifier-naming)

}; // namespace PBR

#endif // RENDERING_PATHTRACER_PBR_HPP