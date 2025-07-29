#include <cmath>
#include <iostream>
#include <linalg/Vec3.hpp>
#include <memory>
#include <stack>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Diagnostics/ScopedTimer.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/MultiThreadedCPU.hpp"
#include "Rendering/PBR.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/RenderTime.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/SingleThreaded.hpp"
#include "Scene/Scene.hpp"
#include "Surface/Material.hpp"

Renderer::Renderer(RenderSettings* render_settings)
    : m_render_time(std::make_unique<RenderTime>()), m_framebuffer(new Framebuffer({1, 1, 1})),
      m_render_settings(render_settings) {}

void Renderer::updateRenderMode() {
  const ImageProperties properties = m_render_settings->getImageProperties();
  m_framebuffer->setFramebufferProperties(properties);

  switch(m_render_settings->getRenderMode()) {
  case RenderMode::SINGLE_THREADED:
    m_render_strategy = std::make_unique<SingleThreaded>();
    break;
  case RenderMode::MULTI_THREADED_CPU: {
    const int          chunk_size   = m_render_settings->getChunkSize();
    const unsigned int thread_count = m_render_settings->getThreadCount();
    m_render_strategy               = std::make_unique<MultiThreadedCPU>(chunk_size, thread_count);
    break;
  }
  default:
    std::cerr << "Unknown render mode. Using single-threaded strategy by default." << '\n';
    m_render_strategy = std::make_unique<SingleThreaded>();
  }
  m_render_strategy->setRenderer(this);
}

bool Renderer::isReadyToRender() const {
  if(m_scene->getCamera() == nullptr) {
    std::cerr << "No camera set in the scene. Cannot render frame." << '\n';
    return false;
  }
  return true;
}

void Renderer::cancelRendering() { m_framebuffer->clearThreadBuffers(); }

bool Renderer::renderFrame() {
  if(!isReadyToRender()) {
    return false;
  }
  updateRenderMode();

  m_stop_requested.store(false);
  const RayEmitterParameters emitter_parameters = {m_scene->getCamera()->getPosition(),
                                                   m_scene->getCamera()->getRotationMatrix(),
                                                   m_scene->getCamera()->getSensorWidth(),
                                                   m_scene->getCamera()->getFocalLength(),
                                                   m_scene->getCamera()->getFocusDistance(),
                                                   m_scene->getCamera()->getLensRadius(),
                                                   static_cast<double>(m_render_settings->getWidth()) /
                                                       static_cast<double>(m_render_settings->getHeight())};
  m_camera_ray_emitter.initializeViewport(emitter_parameters);

  m_scene->buildBVH();

  const bool render_successed = m_render_strategy->render();
  if(!render_successed) {
    cancelRendering();
    return false;
  }

  m_framebuffer->convertToSRGBColorSpace();

  m_render_time->stop();
  std::cout << "Rendering completed in " << m_render_time->getRenderStats().elapsed_time << " seconds.\n";

  ScopedTimer::PrintStats();
  return true;
}

ColorRGBA Renderer::getPixelColor(const PixelCoord& pixel, double dx, double dy, const PixelCoord& subpixel_grid_pos,
                                  double cell_size) const {
  const double   v        = (static_cast<double>(pixel.y) + (subpixel_grid_pos.y + randomUniform01()) * cell_size) * dy;
  const double   u        = (static_cast<double>(pixel.x) + (subpixel_grid_pos.x + randomUniform01()) * cell_size) * dx;
  const Ray      ray      = m_camera_ray_emitter.generateRay(u, v);
  const ColorRGB radiance = getRadiance(ray);
  return ColorRGBA(radiance);
}

void Renderer::renderSample(const PixelCoord& pixel_start, const PixelCoord& pixel_end, double sample_weight,
                            const PixelCoord& subpixel_grid_pos, double cell_size) {
  const double dx = m_render_settings->getDx();
  const double dy = m_render_settings->getDy();

  for(int y = pixel_start.y; y < pixel_end.y; ++y) {
    for(int x = pixel_start.x; x < pixel_end.x; ++x) {
      const ColorRGBA color = getPixelColor({x, y}, dx, dy, subpixel_grid_pos, cell_size);
      m_framebuffer->setPixelColor({x, y}, color, sample_weight);
    }
  }
}

const double* Renderer::getPreviewImage(double factor) {
  m_framebuffer->reduceThreadBuffers();
  m_framebuffer->scaleBufferValues(factor);
  m_framebuffer->convertToSRGBColorSpace();

  return m_framebuffer->getFramebuffer();
}

bool Renderer::isValidHit(const RayHitInfo& hit_info) const {
  const double distance = hit_info.distance;
  return distance > 0.0 && distance <= m_scene->getCamera()->getFarPlane();
}

ColorRGB Renderer::getRadiance(const Ray& ray, const ColorRGB& throughput) const {
  const RayHitInfo hit_info = RayIntersection::getSceneIntersection(ray, m_scene);
  if(!isValidHit(hit_info)) {
    return ColorRGB(m_scene->getSkyboxColor(ray.direction)) * throughput;
  }

  const ColorRGB emitted_light =
      hit_info.material->getEmissive(hit_info.bary_coordinates) * hit_info.material->getEmissiveIntensity();

  const double rr_prob = std::min(1.0, throughput.maxComponent());
  if(randomUniform01() > rr_prob) {
    return emitted_light * throughput;
  }

  const PBR::SurfaceInteraction interaction = {hit_info.normal,
                                               hit_info.tangent,
                                               hit_info.bitangent,
                                               -ray.direction,

                                               ColorRGB(hit_info.material->getDiffuse(hit_info.bary_coordinates)),
                                               hit_info.material->getRoughness(hit_info.bary_coordinates),
                                               hit_info.material->getMetallic(hit_info.bary_coordinates)};

  linalg::Vec3d  outgoing_direction;
  const ColorRGB brdf = PBR::getCookTorranceBrdf(interaction, outgoing_direction);

  const Ray      outgoing_ray   = Ray::FromDirection(hit_info.hit_point, outgoing_direction);
  const ColorRGB new_throughput = throughput * brdf / rr_prob;

  return emitted_light * throughput + getRadiance(outgoing_ray, new_throughput);
}

ColorRGB Renderer::getRadiance(const Ray& initial_ray) const {
  struct Bounce {
    Ray      ray;
    ColorRGB throughput;
  };

  ColorRGB accumulated_radiance(0.0);

  std::stack<Bounce> bounce_stack;
  bounce_stack.push({initial_ray, ColorRGB(1.0)});

  while(!bounce_stack.empty()) {
    const Bounce current = bounce_stack.top();
    bounce_stack.pop();

    const RayHitInfo hit_info = RayIntersection::getSceneIntersection(current.ray, m_scene);
    if(!isValidHit(hit_info)) {
      accumulated_radiance += current.throughput * ColorRGB(m_scene->getSkyboxColor(current.ray.direction));
      continue;
    }

    const ColorRGB emitted_light =
        hit_info.material->getEmissive(hit_info.bary_coordinates) * hit_info.material->getEmissiveIntensity();

    accumulated_radiance += current.throughput * emitted_light;

    const double rr_prob = std::min(1.0, current.throughput.maxComponent());
    if(randomUniform01() >= rr_prob) {
      continue;
    }

    const PBR::SurfaceInteraction interaction = {hit_info.normal,
                                                 hit_info.tangent,
                                                 hit_info.bitangent,
                                                 -current.ray.direction,

                                                 ColorRGB(hit_info.material->getDiffuse(hit_info.bary_coordinates)),
                                                 hit_info.material->getRoughness(hit_info.bary_coordinates),
                                                 hit_info.material->getMetallic(hit_info.bary_coordinates)};

    linalg::Vec3d  outgoing_direction;
    const ColorRGB brdf = PBR::getCookTorranceBrdf(interaction, outgoing_direction);

    const Ray      next_ray        = Ray::FromDirection(hit_info.hit_point, outgoing_direction);
    const ColorRGB next_throughput = current.throughput * brdf / rr_prob;

    bounce_stack.push({next_ray, next_throughput});
  }

  return accumulated_radiance;
}

Renderer::~Renderer() { delete m_framebuffer; }
