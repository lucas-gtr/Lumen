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

inline ColorRGB getBaseReflectance(const ColorRGB& albedo, double metalness, double ior_from, double ior_to) {
  double   r0           = ((ior_from - ior_to) * (ior_from - ior_to)) / ((ior_from + ior_to) * (ior_from + ior_to));
  ColorRGB dielectricF0 = ColorRGB(r0);
  return dielectricF0 * (1.0 - metalness) + albedo * metalness;
}

// NOLINTBEGIN(readability-identifier-naming)
inline ColorRGB getFresnelSchlick(const linalg::Vec3d& half_vector, const linalg::Vec3d& view_direction,
                                  const ColorRGB& F0) {
  const double cos_theta = std::max(0.0, linalg::dot(view_direction, half_vector));
  return F0 + (ColorRGB(1.0) - F0) * std::pow(1.0 - cos_theta, 5);
}

inline double getFresnel(const linalg::Vec3d& half_vector, const linalg::Vec3d& view_direction, double ior_from,
                         double ior_to) {
  const double c  = std::abs(linalg::dot(view_direction, half_vector));
  const double g2 = (ior_to / ior_from) * (ior_to / ior_from) + c * c - 1.0;
  if(g2 < 0.0) {
    return 1.0; // Total internal reflection
  }
  const double g     = std::sqrt(g2);
  const double g_c   = g - c;
  const double g_p   = g + c;
  const double num   = (g_c * g_c);
  const double denom = (g_p * g_p);
  return 0.5 * (num / denom) * (1.0 + ((c * g_p - 1.0) * (c * g_p - 1.0)) / ((c * g_c + 1.0) * (c * g_c + 1.0)));
}

struct BSDFInput {
  linalg::Vec3d incoming_dir;
  linalg::Vec3d normal;

  ColorRGB base_color;
  double   roughness{};
  double   metalness{};
  double   transmission{};
  double   ior_from{};
  double   ior_to{};

  double specular_ratio{};
  double transmission_ratio{};

  double getCriticalAngleCosine() const {
    if(ior_from <= ior_to) {
      return 0.0;
    }
    return std::sqrt(1.0 - (ior_to * ior_to) / (ior_from * ior_from));
  }

  BSDFInput(const linalg::Vec3d& incoming_dir, const linalg::Vec3d& normal, const ColorRGB& base_color,
            double roughness, double metalness, double transmission, double ior_material)
      : incoming_dir(incoming_dir), normal(normal), base_color(base_color), roughness(roughness), metalness(metalness),
        transmission(transmission), ior_from(AIR_IOR), ior_to(ior_material) {

    if(linalg::dot(normal, incoming_dir) < 0.0) {
      this->normal = -normal;
      std::swap(this->ior_from, this->ior_to);
    }

    const ColorRGB F0 = getBaseReflectance(base_color, metalness, ior_from, ior_to);
    const ColorRGB F  = getFresnelSchlick(this->normal, incoming_dir, F0);

    specular_ratio     = metalness + (1.0 - metalness) * F.luminance();
    transmission_ratio = (1.0 - specular_ratio) * transmission;

    const double critical_cos = getCriticalAngleCosine();
    const double cos_theta_i  = std::abs(linalg::dot(this->normal, incoming_dir));

    if(cos_theta_i < critical_cos) {
      specular_ratio += transmission_ratio;
      transmission_ratio = 0.0;
    }
  }
};

inline ColorRGB getLambertianDiffuse(const ColorRGB& diffuse_color) { return diffuse_color * INV_PI; }

inline double getDistributionGgx(const linalg::Vec3d& normal, const linalg::Vec3d& half_vector, double roughness) {
  if(roughness <= 0.0) {
    return normal.isApprox(half_vector, 1e-4) ? 1e6 : 0.0;
  }

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

  const double n_dot_v = std::max(1e-6, linalg::dot(normal, view_dir));
  const double n_dot_l = std::max(1e-6, linalg::dot(normal, light_dir));

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

  const ColorRGB k_s = getFresnelSchlick(half_vector, incoming_dir, F0);
  // const ColorRGB k_s = ColorRGB(getFresnel(half_vector, incoming_dir, input.ior_from, input.ior_to));
  const ColorRGB k_d = (ColorRGB(1.0) - k_s) * (1.0 - input.metalness) * (1.0 - input.transmission);

  const double   specular = getCookTorranceSpecular(incoming_dir, outgoing_direction, input.normal, input.roughness);
  const ColorRGB diffuse  = getLambertianDiffuse(input.base_color);

  return k_d * diffuse + k_s * specular;
}

inline ColorRGB evaluateBtdf(const BSDFInput& input, const linalg::Vec3d& outgoing_direction) {
  const double incident_ior = input.ior_from;
  const double transmit_ior = input.ior_to;

  linalg::Vec3d half_vector = (input.incoming_dir * incident_ior + outgoing_direction * transmit_ior).normalized();
  if(incident_ior < transmit_ior) {
    half_vector = -half_vector;
  }

  const double cos_theta_i_ht = std::max(1e-6, linalg::dot(input.incoming_dir, half_vector));
  const double cos_theta_o_ht = std::max(1e-6, linalg::dot(outgoing_direction, -half_vector));
  const double cos_theta_i_n  = std::max(0.0, linalg::dot(input.incoming_dir, input.normal));
  const double cos_theta_o_n  = std::max(0.0, linalg::dot(outgoing_direction, -input.normal));

  const double angle_factor = (cos_theta_i_ht * cos_theta_o_ht) / (cos_theta_i_n * cos_theta_o_n);
  const double denominator  = (incident_ior * cos_theta_i_ht + transmit_ior * cos_theta_o_ht);

  const double D = getDistributionGgx(input.normal, half_vector, input.roughness);
  const double G = getGeometrySmith(input.normal, input.incoming_dir, outgoing_direction, input.roughness);

  const ColorRGB F0  = getBaseReflectance(input.base_color, input.metalness, input.ior_from, input.ior_to);
  const double   k_t = (ColorRGB(1.0) - getFresnelSchlick(input.normal, input.incoming_dir, F0)).luminance() *
                     (1.0 - input.metalness) * input.transmission;

  // return ColorRGB(input.transmission_ratio * angle_factor);
  return ColorRGB(angle_factor * k_t * D * G * transmit_ior * transmit_ior / (denominator * denominator));
}

inline ColorRGB evaluateBsdf(const BSDFInput& input, const linalg::Vec3d& outgoing_direction) {
  if(linalg::dot(input.normal, outgoing_direction) > 0.0) {
    return evaluateBrdf(input, outgoing_direction);
  } else {
    return evaluateBtdf(input, outgoing_direction);
  }
}
// NOLINTEND(readability-identifier-naming)

}; // namespace PBR

#endif // RENDERING_PATHTRACER_PBR_HPP