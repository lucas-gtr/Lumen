#include <Eigen/Core>
#include <Eigen/Dense>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/MultiThreadedCPU.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/SingleThreaded.hpp"
#include "Scene/Scene.hpp"
#include "Surface/Material.hpp"

Renderer::Renderer(const RenderSettings* render_settings, Scene* scene)
    : m_render_settings(render_settings), m_scene(scene),
      m_framebuffer(new Framebuffer(
          {render_settings->getWidth(), render_settings->getHeight(), render_settings->getChannelCount()})) {
  switch(render_settings->getExecutionMode()) {
  case RenderExecutionMode::SINGLE_THREADED:
    m_render_strategy = std::make_unique<SingleThreaded>();
    break;
  case RenderExecutionMode::MULTI_THREADED_CPU:
    m_render_strategy = std::make_unique<MultiThreadedCPU>(render_settings->getRendererParameters());
    break;
  default:
    std::cerr << "Unknown render execution mode. Using single-threaded strategy by default." << '\n';
    m_render_strategy = std::make_unique<SingleThreaded>();
  }
  m_render_strategy->setRenderer(this);
}

void Renderer::setRenderSettings(const RenderSettings* settings) {
  m_render_settings                = settings;
  const ImageProperties properties = {settings->getWidth(), settings->getHeight(), settings->getChannelCount()};
  m_framebuffer->setFramebufferProperties(properties);
}

bool Renderer::isReadyToRender() const {
  if(m_scene->getCamera() == nullptr) {
    std::cerr << "No camera set in the scene. Cannot render frame." << '\n';
    return false;
  }
  return true;
}

bool Renderer::renderFrame() {
  if(!isReadyToRender()) {
    return false;
  }

  const RayEmitterParameters emitter_parameters = {m_scene->getCamera()->getPosition(),
                                                   m_scene->getCamera()->getRotationMatrix(),
                                                   m_scene->getCamera()->getSensorWidth(),
                                                   m_scene->getCamera()->getFocalLength(),
                                                   m_scene->getCamera()->getFocusDistance(),
                                                   m_scene->getCamera()->getLensRadius(),
                                                   static_cast<double>(m_render_settings->getWidth()) /
                                                       static_cast<double>(m_render_settings->getHeight())};
  m_cameraRayEmitter.initializeViewport(emitter_parameters);

  const auto start_time = std::chrono::high_resolution_clock::now();

  m_scene->buildBVH();

  const bool render_successed = m_render_strategy->render();
  if(!render_successed) {
    std::cerr << "Rendering failed." << '\n';
    return false;
  }

  m_framebuffer->convertToSRGBColorSpace();
  const auto                          end_time        = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> render_duration = end_time - start_time;

  std::cout << "Render time: " << render_duration.count() << " seconds.\n";
  return true;
}

ColorRGBA Renderer::getPixelColor(const PixelCoord& pixel, double dx, double dy, const PixelCoord& subpixel_grid_pos,
                                  double cell_size) const {
  const double v   = (static_cast<double>(pixel.y) + (subpixel_grid_pos.y + randomUniform01()) * cell_size) * dy;
  const double u   = (static_cast<double>(pixel.x) + (subpixel_grid_pos.x + randomUniform01()) * cell_size) * dx;
  const Ray    ray = m_cameraRayEmitter.generateRay(u, v);
  return traceRay(ray);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void Renderer::renderSample(const PixelCoord& pixel_start, const PixelCoord& pixel_end, double sample_weight,
                            // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
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

bool Renderer::isValidHit(const RayHitInfo& hit_info) const {
  const double distance = hit_info.distance;
  return distance >= m_render_settings->getNearPlane() && distance <= m_render_settings->getFarPlane();
}

ColorRGBA Renderer::traceRay(const Ray& ray) const {
  const RayHitInfo hit_info = RayIntersection::getSceneIntersection(ray, m_scene);
  if(!isValidHit(hit_info)) {
    return m_scene->getSkyboxColor(ray.direction);
  }

  const ColorRGBA diffuse_color = hit_info.material->getAlbedo(hit_info.uvCoordinates);

  ColorRGB light_factor = {0.0, 0.0, 0.0};
  for(const auto& light : m_scene->getLightList()) {
    Ray shadow_ray;
    shadow_ray.direction = light->getDirectionFromPoint(hit_info.hitPoint);
    shadow_ray.origin    = hit_info.hitPoint + shadow_ray.direction * RayIntersection::RAY_OFFSET_FACTOR;

    const RayHitInfo shadow_hit = RayIntersection::getSceneIntersection(shadow_ray, m_scene);
    if(isValidHit(shadow_hit)) {
      continue;
    }

    light_factor += light->getLightFactor(hit_info.hitPoint, hit_info.normal);
  }

  ColorRGBA final_color = {diffuse_color.r * light_factor.r, diffuse_color.g * light_factor.g,
                           diffuse_color.b * light_factor.b, diffuse_color.a};
  return final_color;
}

Renderer::~Renderer() { delete m_framebuffer; }
