#ifndef RENDERING_PATHTRACER_PATHTRACER_HPP
#define RENDERING_PATHTRACER_PATHTRACER_HPP

#include <linalg/linalg.hpp>

#include "Rendering/PathTracer/DirectionSampler.hpp"
#include "Rendering/PathTracer/PBR.hpp"

class Scene;

class PathTracer {
private:
  Scene* m_scene;

  bool isValidHit(double distance) const;

  static PBR::BRDFInput CreateBrdfInput(const RayHitInfo& hit_info, const linalg::Vec3d& incoming_dir);

  static linalg::Vec3d Reflect(const linalg::Vec3d& incident, const linalg::Vec3d& normal) {
    return incident - 2 * linalg::dot(incident, normal) * normal;
  }

  static linalg::Vec3d SampleOutgoingDirection(const PBR::BRDFInput& input, const linalg::Mat3d& tbn, double& pdf);

  ColorRGB        computeDirectLighting(const PBR::BRDFInput& input, const linalg::Vec3d& hit_position) const;
  static ColorRGB ComputeBrdfContribution(const PBR::BRDFInput& input, const linalg::Vec3d& outgoing_dir, double pdf);

public:
  PathTracer() = default;

  PathTracer(const PathTracer& other)            = delete;
  PathTracer& operator=(const PathTracer& other) = delete;
  PathTracer(PathTracer&& other)                 = delete;
  PathTracer& operator=(PathTracer&& other)      = delete;

  void setScene(Scene* scene) { m_scene = scene; }

  ColorRGB traceRay(const Ray& ray_in) const;
  ColorRGB traceRayRecursion(const Ray& ray, const ColorRGB& throughput = ColorRGB(1.0), int depth = 0,
                             double prev_brdf_pdf = 1.0, std::vector<double> prev_brdf_pdf_list = {1.0}) const;

  ~PathTracer() = default;
};
#endif // RENDERING_PATHTRACER_PATHTRACER_HPP