#include <cmath>
#include <iostream>
#include <linalg/Vec3.hpp>
#include <memory>
#include <stack>

#include "Core/Color.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/ImageTypes.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Core/ScopedTimer.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/MultiThreadedCPU.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/RenderTime.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/SingleThreaded.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"
#include "Surface/Material.hpp"

Renderer::Renderer(RenderSettings* render_settings)
    : m_framebuffer(new Framebuffer({1, 1})), m_render_settings(render_settings), m_path_tracer() {}

void Renderer::setScene(Scene* scene) {
  m_scene = scene;
  m_path_tracer.setScene(scene);
}

void Renderer::updateRenderMode() {
  const Resolution properties = m_render_settings->getImageResolution();
  m_framebuffer->setResolution(properties);

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

void Renderer::setupRayEmitterParameters() {
  const RayEmitterParameters emitter_parameters = {m_scene->getCamera()->getPosition(),
                                                   m_scene->getCamera()->getRotationMatrix(),
                                                   m_scene->getCamera()->getSensorWidth(),
                                                   m_scene->getCamera()->getFocalLength(),
                                                   m_scene->getCamera()->getFocusDistance(),
                                                   m_scene->getCamera()->getLensRadius(),
                                                   static_cast<double>(m_render_settings->getWidth()) /
                                                       static_cast<double>(m_render_settings->getHeight())};
  m_camera_ray_emitter.initializeViewport(emitter_parameters);
}

bool Renderer::renderFrame() {
  if(!isReadyToRender()) {
    return false;
  }
  updateRenderMode();

  m_stop_requested.store(false);
  setupRayEmitterParameters();

  m_scene->buildBVH();

  const bool render_successed = m_render_strategy->render();
  if(!render_successed) {
    cancelRendering();
    return false;
  }

  m_framebuffer->convertToSRGBColorSpace();

  m_render_time.stop();
  std::cout << "Rendering completed in " << m_render_time.getRenderStats().elapsed_time << " seconds.\n";

  ScopedTimer::PrintStats();
  return true;
}

ColorRGB Renderer::getPixelColor(const PixelCoord& pixel, double dx, double dy, const PixelCoord& subpixel_grid_pos,
                                 double cell_size) const {
  const double   v        = (static_cast<double>(pixel.y) + (subpixel_grid_pos.y + randomUniform01()) * cell_size) * dy;
  const double   u        = (static_cast<double>(pixel.x) + (subpixel_grid_pos.x + randomUniform01()) * cell_size) * dx;
  const Ray      ray      = m_camera_ray_emitter.generateRay(u, v);
  const ColorRGB radiance = m_path_tracer.traceRay(ray);
  return radiance;
}

void Renderer::renderSample(const PixelCoord& pixel_start, const PixelCoord& pixel_end, double sample_weight,
                            const PixelCoord& subpixel_grid_pos, double cell_size) {
  const double dx = m_render_settings->getDx();
  const double dy = m_render_settings->getDy();

  for(int y = pixel_start.y; y < pixel_end.y; ++y) {
    for(int x = pixel_start.x; x < pixel_end.x; ++x) {
      const ColorRGB color = getPixelColor({x, y}, dx, dy, subpixel_grid_pos, cell_size);
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

Renderer::~Renderer() { delete m_framebuffer; }
