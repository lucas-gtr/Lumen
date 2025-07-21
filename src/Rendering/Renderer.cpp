#include <cmath>
#include <iostream>
#include <linalg/Vec3.hpp>
#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/MultiThreadedCPU.hpp"
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

  std::cout << "Rendering scene with " << m_scene->getObjectList().size() << " objects and "
            << m_scene->getLightList().size() << " lights." << '\n';
  const bool render_successed = m_render_strategy->render();
  if(!render_successed) {
    cancelRendering();
    return false;
  }

  m_framebuffer->convertToSRGBColorSpace();

  std::cout << "Rendering completed successfully." << '\n';
  return true;
}

ColorRGBA Renderer::getPixelColor(const PixelCoord& pixel, double dx, double dy, const PixelCoord& subpixel_grid_pos,
                                  double cell_size) const {
  const double v   = (static_cast<double>(pixel.y) + (subpixel_grid_pos.y + randomUniform01()) * cell_size) * dy;
  const double u   = (static_cast<double>(pixel.x) + (subpixel_grid_pos.x + randomUniform01()) * cell_size) * dx;
  const Ray    ray = m_camera_ray_emitter.generateRay(u, v);
  return traceRay(ray);
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

ColorRGBA Renderer::traceRay(const Ray& ray) const {
  const RayHitInfo hit_info = RayIntersection::getSceneIntersection(ray, m_scene);
  if(!isValidHit(hit_info)) {
    return m_scene->getSkyboxColor(ray.direction);
  }

  const ColorRGBA diffuse_color = hit_info.material->getDiffuse(hit_info.uv_coordinates);

  ColorRGB light_factor = {0.0, 0.0, 0.0};
  for(const auto& light : m_scene->getLightList()) {
    const linalg::Vec3d light_direction = light->getDirectionFromPoint(hit_info.hit_point).normalized();
    const Ray           shadow_ray = Ray::FromDirection(hit_info.hit_point + hit_info.normal * 0.01, light_direction);

    const RayHitInfo shadow_hit = RayIntersection::getSceneIntersection(shadow_ray, m_scene);
    if(isValidHit(shadow_hit)) {
      continue;
    }

    light_factor += light->getLightFactor(hit_info.hit_point, hit_info.normal);
  }

  ColorRGBA final_color = {diffuse_color.r * light_factor.r, diffuse_color.g * light_factor.g,
                           diffuse_color.b * light_factor.b, diffuse_color.a};
  return final_color;
}

Renderer::~Renderer() { delete m_framebuffer; }
