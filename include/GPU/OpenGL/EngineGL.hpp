/**
 * @file EngineGL.hpp
 * @brief Header file for the EngineGL class, which implements the OpenGL rendering engine.
 */
#ifndef GPU_OPENGL_ENGINEGL_HPP
#define GPU_OPENGL_ENGINEGL_HPP

// GCOVR_EXCL_START

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "GPU/IEngineGPU.hpp"
#include "GPU/OpenGL/FramebufferGL.hpp"
#include "GPU/OpenGL/Lights/ShadowMapGL.hpp"
#include "GPU/OpenGL/ResourceManagerGL.hpp"
#include "GPU/OpenGL/ShadersGL.hpp"
#include "GPU/OpenGL/WindowGL.hpp"
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
class EngineGL : public IEngineGPU {
private:
  WindowGL          m_window;
  ResourceManagerGL m_resource_manager;

  FramebufferGL m_scenePassFramebuffer;
  FramebufferGL m_resolveFramebuffer;

  ShadersGL m_shadowMap2DProgram;
  ShadersGL m_shadowMapCubeProgram;
  ShadersGL m_scenePassProgram;
  ShadersGL m_skyboxProgram;
  ShadersGL m_postProcessingProgram;

  int         m_shadowMapsSize = DEFAULT_SHADOW_MAP_SIZE;
  ShadowMapGL m_shadowMap2D;
  ShadowMapGL m_shadowMapCube;

  GLuint m_quadVAO = 0U;

  int m_viewport_width  = 0;
  int m_viewport_height = 0;

  float m_exposure = 1.0F;

  void        setupGLFWCallbacks();
  void        loadShaderPrograms();
  static void configureOpenGLStates();
  void        initQuadVAO();

  void processInput();

  void setupLights();

  void setShadowMapSize(int size);
  void setViewportSize(int width, int height);

  void drawShadowMap2D();
  void drawShadowMapCube();
  void drawLightShadowMap2D(const float* lightSpaceMatrix, int indexX, int indexY);
  void drawPointShadowMap(const PointLightGL* light);

  void uploadDirectionalLightData();
  void uploadSpotLightData();
  void uploadPointLightData();

  void configureShadersAndUniforms();
  void renderFrame();
  void blitSceneToResolveFramebuffer();

  void renderScenePass();
  void drawSkybox();
  void applyPostProcessing();

  void displayFPS(float interval);

  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
  static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

public:
  /**
   * @brief Constructor for the EngineGL class.
   * @param width The width of the OpenGL window.
   * @param height The height of the OpenGL window.
   * @param title The title of the OpenGL window.
   * @param scene The Scene object containing the initial objects and lights to be rendered.
   */
  EngineGL(int width, int height, const std::string& title, Scene& scene);

  EngineGL(const EngineGL&)            = delete;
  EngineGL& operator=(const EngineGL&) = delete;
  EngineGL(EngineGL&&)                 = delete;
  EngineGL& operator=(EngineGL&&)      = delete;

  /**
   * @brief Gets the ResourceManagerGL instance.
   * @return A reference to the ResourceManagerGL instance managing OpenGL resources.
   */
  ResourceManagerGL& getResourceManager() { return m_resource_manager; }

  /**
   * @brief Sets the tone mapping algorithm for post-processing.
   * @param toneMapping The ToneMapping enum value to set, determining the tone mapping algorithm used.
   */
  void setToneMapping(ToneMapping toneMapping);

  /**
   * @brief Sets the exposure for post-processing effects.
   * @param exposure The exposure value to set, typically in the range [0.1, 10.0].
   */
  void setExposure(float exposure);

  /**
   * @brief Renders the scene using OpenGL.
   */
  void render() override;

  /**
   * @brief Destructor for the EngineGL class.
   */
  ~EngineGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_ENGINEGL_HPP