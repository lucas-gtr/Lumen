/**
 * @file ShadowMapGL.hpp
 * @brief Header file for the ShadowMapGL class, which manages shadow mapping in OpenGL.
 */
#ifndef GPU_OPENGL_LIGHTS_SHADOWMAPGL_HPP
#define GPU_OPENGL_LIGHTS_SHADOWMAPGL_HPP

// GCOVR_EXCL_START
#include <qopenglfunctions_3_3_core.h>

/**
 * @class ShadowMapGL
 * @brief A class that manages shadow mapping in OpenGL.
 *
 * This class provides functionality to create and manage shadow maps, including
 * initializing 2D and cube map shadow maps, binding the shadow map for rendering,
 * binding the shadow map texture for sampling, and cleaning up resources.
 */
class ShadowMapGL : protected QOpenGLFunctions_3_3_Core {
private:
  unsigned int m_depthMapFBO = 0;
  unsigned int m_depthMap    = 0;

  int m_size = DEFAULT_SHADOW_MAP_SIZE;

  GLenum m_depthMapType = GL_TEXTURE_2D;

  void createFramebuffer();
  void createDepthTexture2D();
  void createDepthTextureCube();
  void attachDepthTexture(GLenum target);
  void validateFramebuffer();

public:
  /**
   * @brief Constructor for ShadowMapGL.
   * @param size The size of the shadow map in pixels (squared)
   */
  explicit ShadowMapGL(int size);

  ShadowMapGL(const ShadowMapGL&)            = delete; ///< Delete copy constructor.
  ShadowMapGL& operator=(const ShadowMapGL&) = delete; ///< Delete copy assignment operator.
  ShadowMapGL(ShadowMapGL&&)                 = delete; ///< Delete move constructor.
  ShadowMapGL& operator=(ShadowMapGL&&)      = delete; ///< Delete move assignment operator.

  /**
   * @brief Initializes a 2D shadow map.
   */
  void initialize2DMap();

  /**
   * @brief Initializes a cube map shadow map.
   */
  void initializeCubeMap();

  /**
   * @brief Binds the shadow map for rendering.
   */
  void bindFramebuffer();

  void resize(int newSize);

  /**
   * @brief Binds the shadow map texture for sampling.
   * @param textureUnit The texture unit to bind the shadow map to.
   */
  void bindTexture(unsigned int textureUnit);

  /**
   * @brief Cleans up resources used by the shadow map.
   */
  void cleanup();

  /**
   * @brief Destructor for ShadowMapGL.
   */
  ~ShadowMapGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_LIGHTS_SHADOWMAPGL_HPP