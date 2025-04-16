#include "Rendering/Renderer.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Ray.hpp"
#include "Core/RayIntersection.hpp"
#include "Core/Scene.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/RenderSettings.hpp"

#include <Eigen/Core>
#include <cstddef>
#include <iostream>

Renderer::Renderer(const RenderSettings* render_settings, const Scene* scene)
    : m_renderSettings(render_settings), m_scene(scene),
      m_framebuffer(new Framebuffer(
          {render_settings->getWidth(), render_settings->getHeight(), render_settings->getChannelCount()})) {}

void Renderer::setRenderSettings(const RenderSettings* settings) {
  m_renderSettings                 = settings;
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
                                                   static_cast<double>(m_renderSettings->getWidth()) /
                                                       static_cast<double>(m_renderSettings->getHeight())};
  m_cameraRayEmitter.initializeViewport(emitter_parameters);

  const double sample_weight = 1.0 / m_renderSettings->getSamplesPerPixel();
  for(int s = 0; s < m_renderSettings->getSamplesPerPixel(); ++s) {
    std::cout << "Sample: " << s + 1 << "/" << m_renderSettings->getSamplesPerPixel() << '\n';
    renderSample(sample_weight);
  }
}

void Renderer::renderSample(double sample_weight) {
  const size_t height = m_renderSettings->getHeight();
  const size_t width  = m_renderSettings->getWidth();

  const double dy = 1.0 / static_cast<double>(height);
  const double dx = 1.0 / static_cast<double>(width);

  for(size_t y = 0; y < height; ++y) {
    const double v = (static_cast<double>(y) + 0.5) * dy;
    for(size_t x = 0; x < width; ++x) {
      const double          u     = (static_cast<double>(x) + 0.5) * dx;
      const Ray             ray   = m_cameraRayEmitter.generateRay(u, v);
      const Eigen::Vector3d color = traceRay(ray);
      m_framebuffer->setPixelColor(x, y, color, sample_weight);
    }
  }
}

Eigen::Vector3d Renderer::traceRay(const Ray& ray) const {
  RayHitInfo closest_hit;
  for(const auto& object : m_scene->getObjectList()) {
    const RayHitInfo hit = RayIntersection::getObjectIntersection(ray, object.get());
    if(hit.distance < closest_hit.distance) {
      closest_hit = hit;
    }
  }

  if(closest_hit.distance < m_renderSettings->getFarPlane() &&
     closest_hit.distance > m_renderSettings->getNearPlane()) {
    return closest_hit.color;
  }

  return m_scene->getBackgroundColor();
}

Renderer::~Renderer() {
  if(m_framebuffer != nullptr) {
    delete m_framebuffer;
    m_framebuffer = nullptr;
  }
}
