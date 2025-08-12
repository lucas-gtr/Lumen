#include <algorithm>
#include <linalg/Mat3.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>
#include <vector>

#include "Core/Color.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Rendering/PathTracer/DirectionSampler.hpp"
#include "Rendering/PathTracer/PBR.hpp"
#include "Rendering/PathTracer/PathTracer.hpp"
#include "Rendering/PathTracer/RayIntersection.hpp"
#include "Scene/LightSample.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"

bool PathTracer::isValidHit(double distance) const {
  return distance > 0.0 && distance <= m_scene->getCamera()->getFarPlane();
}

PBR::BRDFInput PathTracer::CreateBrdfInput(const RayHitInfo& hit_info, const linalg::Vec3d& incoming_dir) {
  return {incoming_dir, hit_info.normal, hit_info.material->getDiffuse(hit_info.bary_coords),
          hit_info.material->getRoughness(hit_info.bary_coords), hit_info.material->getMetalness(hit_info.bary_coords)};
}

linalg::Vec3d PathTracer::SampleOutgoingDirection(const PBR::BRDFInput& brdf_input, const linalg::Mat3d& tbn,
                                                  double& pdf) {

  const linalg::Vec3d& incoming_dir = brdf_input.incoming_dir;

  linalg::Vec3d outgoing_dir;
  if(randomUniform01() < brdf_input.specular_ratio) {
    const linalg::Vec3d half_dir = Sampler::sampleHalfVectorGgx(brdf_input.roughness, tbn);
    outgoing_dir                 = Reflect(-incoming_dir, half_dir);
    pdf = Sampler::pdfHalfVectorGgx(brdf_input.specular_ratio, brdf_input.roughness, brdf_input.incoming_dir,
                                    brdf_input.normal, half_dir);
  } else {
    outgoing_dir = Sampler::sampleCosineHemisphere(tbn);
    pdf          = Sampler::pdfCosineHemisphere(1 - brdf_input.specular_ratio, brdf_input.normal, outgoing_dir);
  }

  return outgoing_dir;
}

ColorRGB PathTracer::computeDirectLighting(const PBR::BRDFInput& brdf_input, const linalg::Vec3d& hit_position) const {
  const LightSample* light_sample = m_scene->getLightSample();
  if(light_sample == nullptr) {
    return ColorRGB(0.0);
  }
  const linalg::Vec3d light_dir = Sampler::sampleLight(light_sample, hit_position);

  const Ray        light_ray = Ray::FromDirection(hit_position, light_dir);
  const RayHitInfo light_hit = RayIntersection::getSceneIntersection(light_ray, m_scene);

  const ColorRGB light_color = light_hit.emitted_light;
  if(light_color == ColorRGB(0.0)) {
    return ColorRGB(0.0);
  }

  const double        pdf      = Sampler::pdfLightSample(m_scene->getLightSampleCount(), light_hit, light_dir);
  std::vector<double> all_pdfs = Sampler::pdfListBrdf(brdf_input.specular_ratio, brdf_input.roughness,
                                                      brdf_input.incoming_dir, brdf_input.normal, light_dir);
  all_pdfs.push_back(pdf);
  const double mis_weight = Sampler::balanceHeuristic(pdf, all_pdfs);

  const ColorRGB brdf_contribution = PathTracer::ComputeBrdfContribution(brdf_input, light_dir, pdf);

  return mis_weight * brdf_contribution * light_color;
}

ColorRGB PathTracer::ComputeBrdfContribution(const PBR::BRDFInput& brdf_input, const linalg::Vec3d& outgoing_dir,
                                             double pdf) {
  const ColorRGB brdf_eval = PBR::evaluateBrdf(brdf_input, outgoing_dir);
  const double   cos_theta = std::max(0.0, linalg::dot(brdf_input.normal, outgoing_dir));

  return brdf_eval * cos_theta / pdf;
}

ColorRGB PathTracer::traceRayRecursion(const Ray& ray_in, const ColorRGB& throughput, int depth, double prev_brdf_pdf,
                                       std::vector<double> prev_brdf_pdf_list) const {
  const RayHitInfo hit = RayIntersection::getSceneIntersection(ray_in, m_scene);

  if(depth > 0) {
    prev_brdf_pdf_list.push_back(Sampler::pdfLightSample(m_scene->getLightSampleCount(), hit, ray_in.direction));
  }
  const double mis_weight = Sampler::balanceHeuristic(prev_brdf_pdf, prev_brdf_pdf_list);

  if(!isValidHit(hit.distance)) {
    return mis_weight * ColorRGB(m_scene->getSkybox()->getColor(ray_in.direction));
  }

  ColorRGB     emission = hit.emitted_light;
  const double rr_prob  = std::min((mis_weight * throughput).maxComponent(), 1.0);
  if(randomUniform01() >= rr_prob) {
    return mis_weight * emission;
  }

  const PBR::BRDFInput brdf_input      = CreateBrdfInput(hit, -ray_in.direction);
  const ColorRGB       direct_lighting = computeDirectLighting(brdf_input, hit.position);

  const linalg::Mat3d tbn          = linalg::Mat3d::FromColumns(hit.tangent, hit.bitangent, hit.normal);
  double              pdf_brdf     = 0.0;
  const linalg::Vec3d outgoing_dir = SampleOutgoingDirection(brdf_input, tbn, pdf_brdf);

  const ColorRGB            brdf_contribution = ComputeBrdfContribution(brdf_input, outgoing_dir, pdf_brdf);
  const std::vector<double> brdf_pdf_list     = Sampler::pdfListBrdf(brdf_input.specular_ratio, brdf_input.roughness,
                                                                     -ray_in.direction, brdf_input.normal, outgoing_dir);

  const ColorRGB next_throughput = throughput * brdf_contribution / rr_prob;

  const Ray      ray_out = Ray::FromDirection(hit.position, outgoing_dir);
  const ColorRGB indirect_lighting =
      brdf_contribution * traceRayRecursion(ray_out, next_throughput, depth + 1, pdf_brdf, brdf_pdf_list);

  emission += (indirect_lighting + direct_lighting) / rr_prob;
  return mis_weight * emission;
}

ColorRGB PathTracer::traceRay(const Ray& ray_in) const {
  ColorRGB            ray_color(1.0);
  int                 depth         = 0;
  double              brdf_pdf      = 1.0;
  std::vector<double> brdf_pdf_list = {1.0};
  Ray                 ray           = ray_in;

  ColorRGB total_radiance(0.0);
  while(true) {
    const RayHitInfo hit = RayIntersection::getSceneIntersection(ray, m_scene);
    if(depth > 0) {
      brdf_pdf_list.push_back(Sampler::pdfLightSample(m_scene->getLightSampleCount(), hit, ray.direction));
    }
    const double mis_weight = Sampler::balanceHeuristic(brdf_pdf, brdf_pdf_list);
    ray_color *= mis_weight;

    if(!isValidHit(hit.distance)) {
      total_radiance += ray_color * ColorRGB(m_scene->getSkybox()->getColor(ray.direction));
      break;
    }

    const double rr_prob = std::min(ray_color.maxComponent(), 1.0);
    if(randomUniform01() >= rr_prob) {
      total_radiance += ray_color * hit.emitted_light;
      break;
    }

    const PBR::BRDFInput brdf_input      = CreateBrdfInput(hit, -ray.direction);
    const ColorRGB       direct_lighting = computeDirectLighting(brdf_input, hit.position);

    const linalg::Mat3d tbn          = linalg::Mat3d::FromColumns(hit.tangent, hit.bitangent, hit.normal);
    const linalg::Vec3d outgoing_dir = SampleOutgoingDirection(brdf_input, tbn, brdf_pdf);

    const ColorRGB brdf_contribution = ComputeBrdfContribution(brdf_input, outgoing_dir, brdf_pdf);

    total_radiance += ray_color * (hit.emitted_light + (direct_lighting / rr_prob));

    ray_color *= brdf_contribution / rr_prob;
    brdf_pdf_list = Sampler::pdfListBrdf(brdf_input.specular_ratio, brdf_input.roughness, -ray.direction,
                                         brdf_input.normal, outgoing_dir);
    ray           = Ray::FromDirection(hit.position, outgoing_dir);
    depth++;
  }

  return total_radiance;
}
