#include <Eigen/Core>
#include <Eigen/Dense>
#include <chrono>
#include <cmath>
#include <iostream>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Surface/Material.hpp"

Renderer::Renderer(const RenderSettings* render_settings, Scene* scene)
    : m_render_settings(render_settings), m_scene(scene),
      m_framebuffer(new Framebuffer(
          {render_settings->getWidth(), render_settings->getHeight(), render_settings->getChannelCount()})) {}

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

void Renderer::renderFrame() {
  if(!isReadyToRender()) {
    return;
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

  const double sample_weight   = 1.0 / m_render_settings->getSamplesPerPixel();
  const int    samples_per_row = static_cast<int>(std::sqrt(m_render_settings->getSamplesPerPixel()));
  const double cell_size       = 1.0 / static_cast<double>(samples_per_row);

  const auto start_time = std::chrono::high_resolution_clock::now();

  m_scene->buildBVH();
  for(int s = 0; s < m_render_settings->getSamplesPerPixel(); ++s) {
    std::cout << "Sample: " << s + 1 << "/" << m_render_settings->getSamplesPerPixel() << '\n';
    const PixelCoord grid_pos{s % samples_per_row, s / samples_per_row};
    renderSample(sample_weight, grid_pos, cell_size);
  }
  m_framebuffer->convertToSRGBColorSpace();
  const auto                          end_time        = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> render_duration = end_time - start_time;

  std::cout << "Render time: " << render_duration.count() << " seconds.\n";
}

void Renderer::renderSample(double sample_weight, PixelCoord grid_pos, double cell_size) {
  const int height = m_render_settings->getHeight();
  const int width  = m_render_settings->getWidth();

  const double dy = 1.0 / static_cast<double>(height);
  const double dx = 1.0 / static_cast<double>(width);

  for(int y = 0; y < height; ++y) {
    for(int x = 0; x < width; ++x) {
      const double          v     = (static_cast<double>(y) + (grid_pos.y + randomUniform01()) * cell_size) * dy;
      const double          u     = (static_cast<double>(x) + (grid_pos.x + randomUniform01()) * cell_size) * dx;
      const Ray             ray   = m_cameraRayEmitter.generateRay(u, v);
      const Eigen::Vector4d color = traceRay(ray);
      m_framebuffer->setPixelColor({x, y}, color, sample_weight);
    }
  }
}

bool Renderer::isValidHit(const RayHitInfo& hit_info) const {
  const double distance = hit_info.distance;
  return distance >= m_render_settings->getNearPlane() && distance <= m_render_settings->getFarPlane();
}

Eigen::Vector4d Renderer::traceRay(const Ray& ray) const {
  const RayHitInfo hit_info = RayIntersection::getSceneIntersection(ray, m_scene);
  if(!isValidHit(hit_info)) {
    return m_scene->getSkyboxColor(ray.direction);
  }

  const Eigen::Vector4d diffuse_color = hit_info.material->getAlbedo(hit_info.uvCoordinates);

  Eigen::Vector3d light_factor = {0.0, 0.0, 0.0};
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

  Eigen::Vector4d final_color;
  final_color.head<3>() = diffuse_color.head<3>().cwiseProduct(light_factor);
  final_color[3]        = diffuse_color[3];
  return final_color;
}

Renderer::~Renderer() { delete m_framebuffer; }
