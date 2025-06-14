/**
 * @file Renderer.hpp
 * @brief Header file for the Renderer class.
 */
#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Rendering/RenderExecution.hpp"
#include "Rendering/RenderSettings.hpp"
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
  Scene*                m_scene;
  Framebuffer*          m_framebuffer;

  std::unique_ptr<RenderStrategy> m_render_strategy;
  CameraRayEmitter                m_cameraRayEmitter;

  ColorRGBA traceRay(const Ray& ray) const;
  bool      isValidHit(const RayHitInfo& hit_info) const;

public:
  /**
   * @brief Constructor for the Renderer class.
   *
   * Initializes the Renderer with the provided render settings and scene.
   *
   * @param render_settings The render settings to be used for rendering.
   * @param scene The scene to be rendered.
   */
  Renderer(const RenderSettings* render_settings, Scene* scene);

  Renderer(const Renderer&)            = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&)                 = delete;
  Renderer& operator=(Renderer&&)      = delete;

  /**
   * @brief Sets the render settings for the renderer.
   *
   * This method allows changing the render settings after the Renderer object has been created.
   *
   * @param settings The new render settings to apply.
   */
  void setRenderSettings(const RenderSettings* settings);

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
   * @brief Checks if the renderer is ready to render.
   *
   * This method checks if all necessary components (such as scene and settings) are properly initialized.
   *
   * @return True if the renderer is ready to render, false otherwise.
   */
  bool isReadyToRender() const;

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