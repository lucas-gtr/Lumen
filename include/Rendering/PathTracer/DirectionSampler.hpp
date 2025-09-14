#ifndef RENDERING_PATHTRACER_DIRECTIONSAMPLER_HPP
#define RENDERING_PATHTRACER_DIRECTIONSAMPLER_HPP

#include <linalg/linalg.hpp>

#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Rendering/PathTracer/PBR.hpp"
#include "Rendering/PathTracer/RayIntersection.hpp"

static constexpr double DOT_TOLERANCE = 1e-6;

namespace Sampler {

inline linalg::Vec3d sampleCosineHemisphere(const linalg::Mat3d& tbn_matrix) {
  const double r1 = randomUniform01();
  const double r2 = randomUniform01();

  const double r     = std::sqrt(r1);
  const double theta = 2.0 * M_PI * r2;

  const double x = r * std::cos(theta);
  const double y = r * std::sin(theta);
  const double z = std::sqrt(std::max(0.0, 1.0 - r1));

  return tbn_matrix * linalg::Vec3d{x, y, z};
}

inline double pdfCosineHemisphere(double probability, const linalg::Vec3d& normal, const linalg::Vec3d& direction) {
  return probability * std::max(0.0, linalg::dot(normal, direction)) / PI;
}

inline linalg::Vec3d sampleHalfVectorGgx(double roughness, const linalg::Mat3d& tbn_matrix) {
  const double alpha = roughness * roughness;
  const double u1    = randomUniform01();
  const double u2    = randomUniform01();

  const double theta = std::atan(alpha * std::sqrt(u1 / (1.0 - u1)));
  const double phi   = 2.0 * M_PI * u2;

  const double sin_theta = std::sin(theta);

  const linalg::Vec3d half_vector = {sin_theta * std::cos(phi), sin_theta * std::sin(phi), std::cos(theta)};
  return tbn_matrix * half_vector;
}

inline double pdfReflectedDirection(double probability, double roughness, const linalg::Vec3d& incident,
                                    const linalg::Vec3d& normal, const linalg::Vec3d& half_vector) {
  const double cos_theta_v = std::max(0.0, linalg::dot(incident, normal));
  if(cos_theta_v == 0.0) {
    return 0.0;
  }
  return probability * PBR::getDistributionGgx(normal, half_vector, roughness) /
         (4.0 * cos_theta_v); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}

inline double pdfRefractedDirection(double probability, const linalg::Vec3d& incident, double roughness,
                                    const linalg::Vec3d& normal, const linalg::Vec3d& direction, double ior_from,
                                    double ior_to) {

  linalg::Vec3d half_vector = (incident * ior_from + direction * ior_to).normalized();
  if(ior_from < ior_to) {
    half_vector = -half_vector;
  }
  const double cos_theta_o = std::max(0.0, linalg::dot(direction, -half_vector));
  if(linalg::dot(direction, normal) >= 0.0) {
    return 0.0;
  }
  const double cos_theta_i = std::max(0.0, linalg::dot(incident, half_vector));
  const double denominator = (ior_from * cos_theta_i + ior_to * cos_theta_o);

  const double numerator = (ior_to * ior_to) * cos_theta_o;

  return probability * PBR::getDistributionGgx(normal, half_vector, roughness) * numerator /
         (denominator * denominator);
}

inline linalg::Vec3d sampleLight(const LightSample* light_sample, const linalg::Vec3d& position) {
  const linalg::Vec3d sample_point = light_sample->randomSample();
  return (sample_point - position).normalized();
}

inline double pdfLightSample(int num_light, const RayHitInfo& hit, const linalg::Vec3d& direction) {
  if(hit.material == nullptr) {
    return 0.0;
  }
  if(hit.material->getEmissive(hit.bary_coords) * hit.material->getEmissiveIntensity() == ColorRGB(0.0)) {
    return 0.0;
  }

  const double cos_light = std::max(DOT_TOLERANCE, dot(direction, -hit.normal));

  return (hit.distance * hit.distance) / (num_light * cos_light * hit.area);
}

inline std::vector<double> pdfListBsdf(PBR::LobeProbabilities lobe_probs, double roughness,
                                       const linalg::Vec3d& incident, const linalg::Vec3d& normal,
                                       const linalg::Vec3d& outgoing_direction, double ior_from, double ior_to) {
  std::vector<double> pdf_list;
  pdf_list.reserve(3);

  const linalg::Vec3d half_vector = (incident + outgoing_direction).normalized();

  pdf_list.push_back(pdfCosineHemisphere(lobe_probs.diffuse, normal, outgoing_direction));
  pdf_list.push_back(pdfReflectedDirection(lobe_probs.specular, roughness, incident, normal, half_vector));
  pdf_list.push_back(pdfRefractedDirection(lobe_probs.transmission, incident, roughness, normal, outgoing_direction,
                                           ior_from, ior_to));

  return pdf_list;
}

inline double balanceHeuristic(double pdf_chosen, const std::vector<double>& pdf_list) {
  double sum = 0.0;
  for(const double p : pdf_list) {
    sum += p;
  }
  return (sum > 0.0) ? pdf_chosen / sum : 0.0;
}

inline double powerHeuristic(double pdf_chosen, const std::vector<double>& pdf_list) {
  double sum = 0.0;
  for(const double p : pdf_list) {
    sum += p * p;
  }
  return (sum > 0.0) ? pdf_chosen * pdf_chosen / sum : 0.0;
}

} // namespace Sampler

#endif // RENDERING_PATHTRACER_DIRECTIONSAMPLER_HPP