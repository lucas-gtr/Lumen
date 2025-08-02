#ifndef RENDERING_PATHTRACER_PATHTRACER_HPP
#define RENDERING_PATHTRACER_PATHTRACER_HPP

#include <linalg/linalg.hpp>

#include "Rendering/PathTracer/DirectionSampler.hpp"
#include "Rendering/PathTracer/PBR.hpp"
#include "Scene/Scene.hpp"

class PathTracer {
private:
  Scene* m_scene;

  ColorRGB      getDirectLighting(const PBR::BRDFInput& input, const linalg::Vec3d& light_dir, double& weight) const;
  linalg::Vec3d getOutgoingDirection(double reflection_probability, double roughness, const linalg::Mat3d& tbn_matrix,
                                     const linalg::Vec3d& normal, const linalg::Vec3d& incident, double& pdf) const;
  bool          isValidHit(double distance) const;

  inline linalg::Vec3d reflect(const linalg::Vec3d& incident, const linalg::Vec3d& normal) const {
    return incident - 2 * linalg::dot(incident, normal) * normal;
  }

public:
  PathTracer() = default;

  PathTracer(const PathTracer& other)            = delete;
  PathTracer& operator=(const PathTracer& other) = delete;
  PathTracer(PathTracer&& other)                 = delete;
  PathTracer& operator=(PathTracer&& other)      = delete;

  void setScene(Scene* scene) { m_scene = scene; }

  ColorRGB traceRay(const Ray& ray, const ColorRGB& throughput = ColorRGB(1.0), int depth = 0,
                    double previous_pdf_brdf = 1.0, std::vector<double> previous_pdf_list = {1.0}) const;
};
#endif // RENDERING_PATHTRACER_PATHTRACER_HPP