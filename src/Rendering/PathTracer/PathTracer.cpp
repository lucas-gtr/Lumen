#include "Rendering/PathTracer/PathTracer.hpp"
#include "Rendering/PathTracer/DirectionSampler.hpp"
#include "Rendering/PathTracer/PBR.hpp"
#include "Rendering/PathTracer/RayIntersection.hpp"

bool PathTracer::isValidHit(double distance) const {
  return distance > 0.0 && distance <= m_scene->getCamera()->getFarPlane();
}

linalg::Vec3d PathTracer::getOutgoingDirection(double reflection_probability, double roughness,
                                               const linalg::Mat3d& tbn_matrix, const linalg::Vec3d& normal,
                                               const linalg::Vec3d& incident, double& pdf) const {

  linalg::Vec3d outgoing_direction;
  linalg::Vec3d half_vector;
  if(randomUniform01() < reflection_probability) {
    half_vector        = Sampler::sampleHalfVectorGgx(roughness, tbn_matrix);
    outgoing_direction = reflect(-incident, half_vector);
    pdf                = Sampler::pdfHalfVectorGgx(reflection_probability, roughness, incident, normal, half_vector);
  } else {
    outgoing_direction = Sampler::sampleCosineHemisphere(tbn_matrix);
    half_vector        = (incident + outgoing_direction).normalized();
    pdf                = Sampler::pdfCosineHemisphere(1 - reflection_probability, normal, outgoing_direction);
  }

  return outgoing_direction;
}

ColorRGB PathTracer::getDirectLighting(const PBR::BRDFInput& input, const linalg::Vec3d& light_dir, double& pdf) const {

  const Ray light_ray = Ray::FromDirection(input.hit_point, light_dir);

  const RayHitInfo light_hit = RayIntersection::getSceneIntersection(light_ray, m_scene);

  if(light_hit.material == nullptr) {
    pdf = 1.0;
    return ColorRGB(0.0);
  }
  const ColorRGB light_color =
      light_hit.material->getEmissive(light_hit.bary_coordinates) * light_hit.material->getEmissiveIntensity();
  if(light_color.maxComponent() == 0.0) {
    pdf = 1.0;
    return ColorRGB(0.0);
  }

  pdf = Sampler::pdfLightSample(2, light_hit, light_dir);

  const double cos_theta = std::max(0.0, dot(input.normal, light_dir));

  const ColorRGB brdf_color = PBR::evaluateBrdf(input, light_dir);
  return brdf_color * light_color * cos_theta / pdf;
}

ColorRGB PathTracer::traceRay(const Ray& ray, const ColorRGB& throughput, int depth, double previous_pdf_brdf,
                              std::vector<double> pdf_list) const {
  RayHitInfo hit_info = RayIntersection::getSceneIntersection(ray, m_scene);
  if(!isValidHit(hit_info.distance)) {
    return ColorRGB(0.0);
  }

  const ColorRGB emitted_light =
      hit_info.material->getEmissive(hit_info.bary_coordinates) * hit_info.material->getEmissiveIntensity();

  ColorRGB radiance       = emitted_light;
  double   pdf_light_brdf = 0.0;
  if(radiance.maxComponent() > 0.0 && depth > 0) {
    pdf_list.push_back(Sampler::pdfLightSample(2, hit_info, ray.direction));
  }
  // if(depth > 0) radiance = ColorRGB(0.0);

  const double weight_brdf = Sampler::balanceHeuristic(previous_pdf_brdf, pdf_list);

  const double rr_prob = throughput.maxComponent();
  if(randomUniform01() >= rr_prob) {
    return weight_brdf * radiance;
  }

  const linalg::Vec3d incident     = -ray.direction;
  const LightSample*  light_sample = m_scene->getLightSample();

  const PBR::BRDFInput brdf_input = {hit_info.hit_point,
                                     incident,
                                     hit_info.normal,
                                     hit_info.material->getDiffuse(hit_info.bary_coordinates),
                                     hit_info.material->getRoughness(hit_info.bary_coordinates),
                                     hit_info.material->getMetalness(hit_info.bary_coordinates)};

  const linalg::Mat3d tbn = linalg::Mat3d::FromColumns(hit_info.tangent, hit_info.bitangent, hit_info.normal);
  const ColorRGB      F0  = PBR::getBaseReflectance(brdf_input.base_color, brdf_input.metalness);
  const ColorRGB      F   = PBR::getFresnelSchlick(brdf_input.normal, incident, F0);
  const double        reflection_prob = brdf_input.metalness + (1.0 - brdf_input.metalness) * (F.r + F.g + F.b) / 3.0;

  // === Direct lighting ===
  const linalg::Vec3d light_dir = Sampler::sampleLight(light_sample, hit_info.hit_point);
  const linalg::Vec3d h_light   = (incident + light_dir).normalized();

  const double pdf_diff_light = Sampler::pdfCosineHemisphere(1 - reflection_prob, brdf_input.normal, light_dir);
  const double pdf_spec_light =
      Sampler::pdfHalfVectorGgx(reflection_prob, brdf_input.roughness, incident, brdf_input.normal, h_light);
  double pdf_light = 1.0;

  const ColorRGB direct_lighting = getDirectLighting(brdf_input, light_dir, pdf_light);
  const double   weight_light    = Sampler::balanceHeuristic(pdf_light, {pdf_diff_light, pdf_spec_light, pdf_light});

  // === Indirect lighting ===
  double              pdf_brdf = 0.0;
  const linalg::Vec3d outgoing_dir =
      getOutgoingDirection(reflection_prob, brdf_input.roughness, tbn, brdf_input.normal, incident, pdf_brdf);
  if(pdf_brdf <= 0.0) {
    std::cout << "Warning: pdf_brdf is zero or negative, skipping indirect lighting." << std::endl;
    return weight_brdf * (radiance + direct_lighting) / rr_prob;
  }

  const linalg::Vec3d h_brdf = (incident + outgoing_dir).normalized();

  const double pdf_diff_brdf = Sampler::pdfCosineHemisphere(1 - reflection_prob, brdf_input.normal, outgoing_dir);
  const double pdf_spec_brdf =
      Sampler::pdfHalfVectorGgx(reflection_prob, brdf_input.roughness, incident, brdf_input.normal, h_brdf);

  const std::vector<double> pdf_list = {pdf_diff_brdf, pdf_spec_brdf};

  const ColorRGB brdf              = PBR::evaluateBrdf(brdf_input, outgoing_dir);
  const double   cos_theta         = std::max(0.0, linalg::dot(hit_info.normal, outgoing_dir));
  const ColorRGB outgoing_radiance = brdf * cos_theta / pdf_brdf;
  const ColorRGB new_throughput    = throughput * outgoing_radiance / rr_prob;

  const Ray      next_ray = Ray::FromDirection(hit_info.hit_point, outgoing_dir);
  const ColorRGB indirect_lighting =
      traceRay(next_ray, new_throughput, depth + 1, pdf_brdf, pdf_list) * outgoing_radiance;

  // === Combinaison finale ===
  radiance += (indirect_lighting + weight_light * direct_lighting) / rr_prob;
  return weight_brdf * radiance;
}