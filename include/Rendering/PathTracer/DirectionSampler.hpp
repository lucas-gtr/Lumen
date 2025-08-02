#ifndef RENDERING_PATHTRACER_DIRECTIONSAMPLER_HPP
#define RENDERING_PATHTRACER_DIRECTIONSAMPLER_HPP

#include <linalg/linalg.hpp>

#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Rendering/PathTracer/PBR.hpp"
#include "Rendering/PathTracer/RayIntersection.hpp"

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

  linalg::Vec3d half_vector = {sin_theta * std::cos(phi), sin_theta * std::sin(phi), std::cos(theta)};
  return tbn_matrix * half_vector;
}

inline linalg::Vec3d sampleLight(const LightSample* light_sample, const linalg::Vec3d& hit_point) {
  const linalg::Vec3d sample_point = light_sample->random_sample();
  return (sample_point - hit_point).normalized();
}

inline double pdfHalfVectorGgx(double probability, double roughness, const linalg::Vec3d& incident,
                               const linalg::Vec3d& normal, const linalg::Vec3d& half_vector) {
  return probability * PBR::getDistributionGgx(normal, half_vector, roughness) /
         (4.0 * std::max(1e-6, linalg::dot(incident, normal)));
}

inline double pdfLightSample(int num_light, const RayHitInfo& hit, const linalg::Vec3d& direction) {
  const double cos_light = std::max(1e-6, dot(direction, -hit.normal));

  return (hit.distance * hit.distance) / (num_light * cos_light * hit.area);
}

inline double balanceHeuristic(double pdf_chosen, const std::vector<double>& pdf_list) {
  double sum = 0.0;
  for(double p : pdf_list) {
    sum += p;
  }
  return (sum > 0.0) ? pdf_chosen / sum : 0.0;
}

inline double powerHeuristic(double pdf_chosen, const std::vector<double>& pdf_list) {
  double sum = 0.0;
  for(double p : pdf_list) {
    sum += p * p;
  }
  return (sum > 0.0) ? pdf_chosen * pdf_chosen / sum : 0.0;
}

} // namespace Sampler

#endif // RENDERING_PATHTRACER_DIRECTIONSAMPLER_HPP