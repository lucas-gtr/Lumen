/**
 * @file Renderer.hpp
 * @brief Header file for the Renderer class.
 */
#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Observer.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/RenderStrategy.hpp"
#include "Rendering/RenderTime.hpp"
#include "Scene/Scene.hpp"

/**
 * @class Renderer
 * @brief The Renderer class is responsible for rendering a scene using the given render settings and camera setup.
 *
 * It manages the rendering process, including ray tracing and rendering each frame. It uses various components like
 * RenderSettings, Scene, CameraRayEmitter, and Framebuffer for this purpose.
 */
class Renderer {
private:
  const RenderSettings* m_render_settings;
  Scene*                m_scene = nullptr;
  Framebuffer*          m_framebuffer;

  std::unique_ptr<RenderStrategy> m_render_strategy;
  CameraRayEmitter                m_camera_ray_emitter;

  std::unique_ptr<RenderTime> m_render_time;

  std::atomic<bool> m_stop_requested = false;

  Observer<double> m_render_progress_observer;

  ColorRGB getRadiance(const Ray& ray) const;
  ColorRGB getRadiance(const Ray& ray, const ColorRGB& throughput) const;
  bool     isValidHit(const RayHitInfo& hit_info) const;
  void     cancelRendering();

  void updateRenderMode();

public:
  /**
   * @brief Constructor for the Renderer class.
   * @param render_settings The render settings to be used for rendering.
   */
  explicit Renderer(RenderSettings* render_settings);

  Renderer(const Renderer&)            = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&)                 = delete;
  Renderer& operator=(Renderer&&)      = delete;

  /**
   * @brief Gets the observer for render progress updates.
   * @return A reference to the observer that notifies about render progress.
   */
  Observer<double>& getRenderProgressObserver() { return m_render_progress_observer; }

  /**
   * @brief Gets the render time statistics.
   * @return A pointer to the RenderTime object containing render statistics.
   */
  RenderTime* getRenderTime() const { return m_render_time.get(); }

  /**
   * @brief Sets the scene to be rendered.
   *
   * This method allows changing the scene after the Renderer object has been created.
   *
   * @param scene The new scene to render.
   */
  void setScene(Scene* scene) { this->m_scene = scene; }

  /**
   * @brief Gets the current render settings.
   *
   * @return A constant reference to the current render settings.
   */
  const RenderSettings& getRenderSettings() const { return *m_render_settings; }

  /**
   * @brief Gets the current scene.
   *
   * @return A constant reference to the current scene.
   */
  const Scene& getScene() const { return *m_scene; }

  /**
   * @brief Gets the framebuffer used for storing the rendered image.
   *
   * @return A pointer to the framebuffer.
   */
  Framebuffer* getFramebuffer() const { return m_framebuffer; }

  /**
   * @brief Gets the data of the framebuffer.
   * @return A pointer to the framebuffer data.
   */
  const double* getFramebufferData() const { return m_framebuffer->getFramebuffer(); }

  /**
   * @brief Gets the preview image of the rendered scene.
   * @param factor The scaling factor for the preview image.
   * @return A pointer to the scaled framebuffer data.
   */
  const double* getPreviewImage(double factor);

  /**
   * @brief Checks if the renderer is ready to render.
   * @return True if the renderer is ready to render, false otherwise.
   */
  bool isReadyToRender() const;

  /**
   * @brief Cancels the current rendering process.
   */
  void requestStop() { m_stop_requested.store(true); }

  /**
   * @brief Checks if a stop has been requested for the rendering process.
   * @return True if a stop has been requested, false otherwise.
   */
  bool isStopRequested() const { return m_stop_requested.load(); }

  /**
   * @brief Gets the color of a pixel at a specific subpixel grid position.
   *
   * This method calculates the color of a pixel based on its position and the subpixel grid position.
   *
   * @param pixel The pixel coordinates.
   * @param dx The horizontal offset for the subpixel grid.
   * @param dy The vertical offset for the subpixel grid.
   * @param subpixel_grid_pos The subpixel grid position within the pixel.
   * @param cell_size The size of the cell in the subpixel grid.
   * @return The color of the pixel as a ColorRGBA object.
   */
  ColorRGBA getPixelColor(const PixelCoord& pixel, double dx, double dy, const PixelCoord& subpixel_grid_pos,
                          double cell_size) const;

  /**
   * @brief Renders a sample at a specific grid position.
   *
   * This method renders a single sample based on the provided weight, grid position, and cell size.
   *
   * @param pixel_start The starting pixel coordinates for the sample.
   * @param pixel_end The ending pixel coordinates for the sample.
   * @param sample_weight The weight of the sample.
   * @param subpixel_grid_pos The subpixel grid position within the pixel.
   * @param cell_size The size of the cell in the subpixel grid.
   */
  void renderSample(const PixelCoord& pixel_start, const PixelCoord& pixel_end, double sample_weight,
                    const PixelCoord& subpixel_grid_pos, double cell_size);

  /**
   * @brief Renders a frame of the scene.
   *
   * This method renders a full frame based on the current settings, scene, and camera setup.
   */
  bool renderFrame();

  ~Renderer(); ///< Destructor to clean up resources.
};

#endif // RENDERING_RENDERER_HPP