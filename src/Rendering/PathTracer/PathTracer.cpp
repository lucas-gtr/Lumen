#include <algorithm>
#include <linalg/Mat3.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>
#include <vector>

#include "Core/Color.hpp"
#include "Core/Config.hpp"
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

PBR::BSDFInput PathTracer::CreateBsdfInput(const RayHitInfo& hit_info, const linalg::Vec3d& incoming_dir) {
  return {incoming_dir,
          hit_info.normal,
          hit_info.material->getDiffuse(hit_info.bary_coords),
          hit_info.material->getRoughness(hit_info.bary_coords),
          hit_info.material->getMetalness(hit_info.bary_coords),
          hit_info.material->getTransmission(hit_info.bary_coords),
          hit_info.material->getIndexOfRefraction(),
          hit_info.distance};
}

linalg::Vec3d PathTracer::SampleOutgoingDirection(const PBR::BSDFInput&         bsdf_input,
                                                  const PBR::LobeProbabilities& lobe_probs, const linalg::Mat3d& tbn,
                                                  const linalg::Vec3d& micro_normal, double& pdf) {

  const linalg::Vec3d& incoming_dir = bsdf_input.incoming_dir;

  linalg::Vec3d outgoing_dir;

  const double rand = randomUniform01();

  if(rand < lobe_probs.specular) {
    outgoing_dir = linalg::reflect(-incoming_dir, micro_normal);
    if(linalg::dot(outgoing_dir, bsdf_input.normal) <= 0.0) {
      pdf = 0.0;
      return linalg::Vec3d(0.0);
    }
    pdf = Sampler::pdfReflectedDirection(lobe_probs.specular, bsdf_input.roughness, bsdf_input.incoming_dir,
                                         bsdf_input.normal, micro_normal);
  } else if(rand < lobe_probs.specular + lobe_probs.transmission) {
    outgoing_dir = linalg::refract(-incoming_dir, micro_normal, bsdf_input.ior_from / bsdf_input.ior_to);
    pdf = Sampler::pdfRefractedDirection(lobe_probs.transmission, incoming_dir, bsdf_input.roughness, bsdf_input.normal,
                                         outgoing_dir, bsdf_input.ior_from, bsdf_input.ior_to);
  } else {
    outgoing_dir = Sampler::sampleCosineHemisphere(tbn);
    pdf          = Sampler::pdfCosineHemisphere(lobe_probs.diffuse, bsdf_input.normal, outgoing_dir);
  }

  return outgoing_dir;
}

ColorRGB PathTracer::computeDirectLighting(const PBR::BSDFInput& bsdf_input, const PBR::LobeProbabilities& lobe_probs,
                                           const linalg::Vec3d& hit_position) const {
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

  const double        pdf = Sampler::pdfLightSample(m_scene->getLightSampleCount(), light_hit, light_dir);
  std::vector<double> all_pdfs =
      Sampler::pdfListBsdf(lobe_probs, bsdf_input.roughness, bsdf_input.incoming_dir, bsdf_input.normal, light_dir,
                           bsdf_input.ior_from, bsdf_input.ior_to);
  all_pdfs.push_back(pdf);
  const double mis_weight = Sampler::balanceHeuristic(pdf, all_pdfs);

  const ColorRGB bsdf_contribution = PathTracer::ComputeBsdfContribution(bsdf_input, light_dir, pdf);

  return mis_weight * bsdf_contribution * light_color * (1.0 - bsdf_input.transmission);
}

ColorRGB PathTracer::ComputeBsdfContribution(const PBR::BSDFInput& bsdf_input, const linalg::Vec3d& outgoing_dir,
                                             double pdf) {
  if(pdf == 0.0) {
    return ColorRGB(0.0);
  }

  const ColorRGB bsdf_eval = PBR::evaluateBsdf(bsdf_input, outgoing_dir);
  const double   cos_theta = std::abs(linalg::dot(bsdf_input.normal, outgoing_dir));

  return bsdf_eval * cos_theta / pdf;
}

ColorRGB PathTracer::traceRayRecursion(const Ray& ray_in, const ColorRGB& throughput, int depth, double prev_bsdf_pdf,
                                       std::vector<double> prev_bsdf_pdf_list) const {
  const RayHitInfo hit = RayIntersection::getSceneIntersection(ray_in, m_scene);

  if(depth > 0) {
    prev_bsdf_pdf_list.push_back(Sampler::pdfLightSample(m_scene->getLightSampleCount(), hit, ray_in.direction));
  }
  if(depth >= DEFAULT_MAX_BOUNCES) {
    return ColorRGB(0.0);
  }
  const double mis_weight = Sampler::balanceHeuristic(prev_bsdf_pdf, prev_bsdf_pdf_list);

  if(!isValidHit(hit.distance)) {
    return mis_weight * ColorRGB(m_scene->getSkybox()->getColor(ray_in.direction));
  }

  ColorRGB     emission = hit.emitted_light;
  const double rr_prob  = std::min((mis_weight * throughput).maxComponent(), 1.0);
  if(randomUniform01() >= rr_prob) {
    return mis_weight * emission;
  }

  const PBR::BSDFInput bsdf_input   = CreateBsdfInput(hit, -ray_in.direction);
  const linalg::Mat3d  tbn          = linalg::Mat3d::FromColumns(hit.tangent, hit.bitangent, hit.normal);
  const linalg::Vec3d& micro_normal = Sampler::sampleHalfVectorGgx(bsdf_input.roughness, tbn);

  const PBR::LobeProbabilities lobe_probs{bsdf_input, micro_normal};

  const ColorRGB direct_lighting = computeDirectLighting(bsdf_input, lobe_probs, hit.position);

  double              pdf_bsdf     = 0.0;
  const linalg::Vec3d outgoing_dir = SampleOutgoingDirection(bsdf_input, lobe_probs, tbn, micro_normal, pdf_bsdf);

  if(outgoing_dir == linalg::Vec3d(0.0)) {
    return mis_weight * (emission + direct_lighting / rr_prob);
  }

  const ColorRGB            bsdf_contribution = ComputeBsdfContribution(bsdf_input, outgoing_dir, pdf_bsdf);
  const std::vector<double> bsdf_pdf_list =
      Sampler::pdfListBsdf(lobe_probs, bsdf_input.roughness, -ray_in.direction, bsdf_input.normal, outgoing_dir,
                           bsdf_input.ior_from, bsdf_input.ior_to);

  const ColorRGB next_throughput = throughput * bsdf_contribution / rr_prob;

  const Ray      ray_out = Ray::FromDirection(hit.position, outgoing_dir);
  const ColorRGB indirect_lighting =
      bsdf_contribution * traceRayRecursion(ray_out, next_throughput, depth + 1, pdf_bsdf, bsdf_pdf_list);

  emission += (indirect_lighting + direct_lighting) / rr_prob;
  return mis_weight * emission;
}

ColorRGB PathTracer::traceRay(const Ray& ray_in) const {
  ColorRGB            ray_color(1.0);
  int                 depth         = 0;
  double              bsdf_pdf      = 1.0;
  std::vector<double> bsdf_pdf_list = {1.0};
  Ray                 ray           = ray_in;

  ColorRGB total_radiance(0.0);
  while(depth < DEFAULT_MAX_BOUNCES) {
    const RayHitInfo hit = RayIntersection::getSceneIntersection(ray, m_scene);
    if(depth > 0) {
      bsdf_pdf_list.push_back(Sampler::pdfLightSample(m_scene->getLightSampleCount(), hit, ray.direction));
    }
    const double mis_weight = Sampler::balanceHeuristic(bsdf_pdf, bsdf_pdf_list);
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

    const PBR::BSDFInput bsdf_input   = CreateBsdfInput(hit, -ray.direction);
    const linalg::Mat3d  tbn          = linalg::Mat3d::FromColumns(hit.tangent, hit.bitangent, bsdf_input.normal);
    const linalg::Vec3d& micro_normal = Sampler::sampleHalfVectorGgx(bsdf_input.roughness, tbn);

    const PBR::LobeProbabilities lobe_probs{bsdf_input, micro_normal};

    const ColorRGB direct_lighting = computeDirectLighting(bsdf_input, lobe_probs, hit.position);

    total_radiance += ray_color * (hit.emitted_light + (direct_lighting / rr_prob));

    const linalg::Vec3d outgoing_dir = SampleOutgoingDirection(bsdf_input, lobe_probs, tbn, micro_normal, bsdf_pdf);

    if(outgoing_dir == linalg::Vec3d(0.0)) {
      break;
    }

    const ColorRGB bsdf_contribution = ComputeBsdfContribution(bsdf_input, outgoing_dir, bsdf_pdf);

    ray_color *= bsdf_contribution / rr_prob;
    bsdf_pdf_list = Sampler::pdfListBsdf(lobe_probs, bsdf_input.roughness, -ray.direction, bsdf_input.normal,
                                         outgoing_dir, bsdf_input.ior_from, bsdf_input.ior_to);
    ray           = Ray::FromDirection(hit.position, outgoing_dir);
    depth++;
  }

  return total_radiance;
}
