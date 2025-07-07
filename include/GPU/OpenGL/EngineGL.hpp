/**
 * @file EngineGL.hpp
 * @brief Header file for the EngineGL class, which implements the OpenGL rendering engine.
 */
#ifndef GPU_OPENGL_ENGINEGL_HPP
#define GPU_OPENGL_ENGINEGL_HPP

// GCOVR_EXCL_START

#include <memory>
#include <qopenglfunctions_3_3_core.h>
#include <string>

#include "GPU/IEngineGPU.hpp"
#include "GPU/OpenGL/FramebufferGL.hpp"
#include "GPU/OpenGL/Lights/ShadowMapGL.hpp"
#include "GPU/OpenGL/ResourceManagerGL.hpp"
#include "GPU/OpenGL/ShadersGL.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "Scene/Scene.hpp"

/**
 * @class EngineGL
 * @brief Implements the OpenGL rendering engine, handling rendering of 3D scenes with support for shadows, skyboxes,
 * and post-processing.
 *
 * This class manages the OpenGL context, handles input, and performs rendering operations such as
 * drawing shadow maps, scene passes, skyboxes, and applying post-processing effects.
 */
class EngineGL : public IEngineGPU, protected QOpenGLFunctions_3_3_Core {
private:
  std::unique_ptr<ResourceManagerGL> m_resource_manager = nullptr;

  std::unique_ptr<FramebufferGL> m_scene_pass_framebuffer = nullptr;
  std::unique_ptr<FramebufferGL> m_resolve_framebuffer    = nullptr;

  ShadersGL m_shadow_map_2_d_program;
  ShadersGL m_shadow_map_cube_program;
  ShadersGL m_scene_pass_program;
  ShadersGL m_outline_program;
  ShadersGL m_skybox_program;
  ShadersGL m_viewport_grid_program;
  ShadersGL m_post_processing_program;

  int                          m_shadow_map_size = DEFAULT_SHADOW_MAP_SIZE;
  std::unique_ptr<ShadowMapGL> m_shadow_map_2_d   = nullptr;
  std::unique_ptr<ShadowMapGL> m_shadow_map_cube = nullptr;

  GLuint m_quad_vao = 0U;

  int m_viewport_width  = 0;
  int m_viewport_height = 0;

  bool m_draw_skybox = false;

  bool m_dynamic_lighting   = false;
  bool m_dynamic_shadow_map = false;

  ToneMapping m_tone_mapping = ToneMapping::NONE;
  float       m_exposure    = 1.0F;

  void loadShaderPrograms();
  void configureOpenGLStates();
  void initQuadVAO();

  void drawShadowMap2D();
  void drawShadowMapCube();
  void drawLightShadowMap2D(const float* light_space_matrix, int index_x, int index_y);
  void drawPointShadowMap(const PointLightGL* light);

  void uploadLightsData();
  void uploadDirectionalLightsData();
  void uploadSpotLightsData();
  void uploadPointLightsData();

  void configureShadersAndUniforms();
  void blitSceneToResolveFramebuffer();

  void renderScenePass();
  void drawOutline();
  void drawSkybox();
  void drawViewportGrid();
  void applyPostProcessing();

public:
  /**
   * @brief Constructor for the EngineGL class.
   * @param width The width of the OpenGL window.
   * @param height The height of the OpenGL window.
   */
  EngineGL(int width, int height);

  EngineGL(const EngineGL&)            = delete;
  EngineGL& operator=(const EngineGL&) = delete;
  EngineGL(EngineGL&&)                 = delete;
  EngineGL& operator=(EngineGL&&)      = delete;

  /**
   * @brief Initializes the OpenGL context and prepares the engine for rendering.
   */
  void initialize(Scene* scene);

  /**
   * @brief Gets the ResourceManagerGL instance.
   * @return A reference to the ResourceManagerGL instance managing OpenGL resources.
   */
  ResourceManagerGL* getResourceManager() { return m_resource_manager.get(); }

  /**
   * @brief Sets the size of the shadow maps used for rendering shadows.
   * @param size The size of the shadow maps in pixels (squared).
   */
  void setShadowMapSize(int size);

  /**
   * @brief Enables or disables the skybox rendering in the scene.
   */
  void setDrawSkybox(bool enabled) { m_draw_skybox = enabled; }

  /**
   * @brief Checks if the skybox rendering is enabled.
   * @return True if the skybox is enabled, false otherwise.
   */
  bool isSkyboxEnabled() const { return m_draw_skybox; }

  /**
   * @brief Gets the size of the shadow maps used for rendering shadows.
   * @return The size of the shadow maps in pixels.
   */
  int getShadowMapSize() const { return m_shadow_map_size; }

  /**
   * @brief Sets the viewport size for rendering.
   * @param width The width of the viewport.
   * @param height The height of the viewport.
   */
  void setViewportSize(int width, int height);

  /**
   * @brief Enables or disables dynamic lighting in the scene.
   * @param enabled True to enable dynamic lighting, false to disable it.
   */
  void setDynamicLighting(bool enabled);

  /**
   * @brief Checks if dynamic lighting is enabled in the scene.
   * @return True if dynamic lighting is enabled, false otherwise.
   */
  bool isDynamicLightingEnabled() const { return m_dynamic_lighting; }

  /**
   * @brief Enables or disables dynamic shadow mapping in the scene.
   * @param enabled True to enable dynamic shadow mapping, false to disable it.
   */
  void setDynamicShadowMap(bool enabled);

  /**
   * @brief Checks if dynamic shadow mapping is enabled in the scene.
   * @return True if dynamic shadow mapping is enabled, false otherwise.
   */
  bool isDynamicShadowMapEnabled() const { return m_dynamic_shadow_map; }

  /**
   * @brief Sets the tone mapping algorithm for post-processing.
   * @param tone_mapping The ToneMapping enum value to set, determining the tone mapping algorithm used.
   */
  void setToneMapping(ToneMapping tone_mapping);

  /**
   * @brief Gets the current tone mapping algorithm used for post-processing.
   * @return A constant reference to the ToneMapping enum value representing the current tone mapping algorithm.
   */
  const ToneMapping& getToneMapping() const { return m_tone_mapping; }

  /**
   * @brief Sets the exposure for post-processing effects.
   * @param exposure The exposure value to set, typically in the range [0.1, 10.0].
   */
  void setExposure(float exposure);

  /**
   * @brief Gets the current exposure value used for post-processing effects.
   * @return The current exposure value
   */
  float getExposure() const { return m_exposure; }

  /**
   * @brief Bakes the lights in the scene, preparing them for rendering.
   */
  void bakeLights();

  /**
   * @brief Sets up the OpenGL rendering context and prepares the engine for rendering.
   */
  void setupRendering();

  /**
   * @brief Renders a single frame of the scene.
   */
  void renderFrame(unsigned int default_ramebuffer);

  /**
   * @brief Destructor for the EngineGL class.
   */
  ~EngineGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_ENGINEGL_HPP