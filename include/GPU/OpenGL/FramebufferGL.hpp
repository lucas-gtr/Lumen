/**
 * @file FramebufferGL.hpp
 * @brief Header file for the FramebufferGL class, which manages OpenGL framebuffers.
 */
#ifndef GPU_OPENGL_FRAMEBUFFERGL_HPP
#define GPU_OPENGL_FRAMEBUFFERGL_HPP

// GCOVR_EXCL_START

#include <cstddef>
#include <qopenglfunctions_3_3_core.h>
#include <vector>

/**
 * @class FramebufferGL
 * @brief A class that manages OpenGL framebuffers, including color and depth attachments.
 *
 * This class provides methods to create, bind, resize, and manage framebuffers with multiple color attachments
 * and an optional depth attachment. It supports multisampling for improved rendering quality.
 */
class FramebufferGL : protected QOpenGLFunctions_3_3_Core {
private:
  GLuint              m_framebufferID = 0;
  std::vector<GLuint> m_colorAttachments;
  GLuint              m_depthRenderbuffer = 0;

  int m_width               = 0;
  int m_height              = 0;
  int m_numColorAttachments = 0;
  int m_multisampleCount    = 1;

  void createFramebuffer();

  void createTexture2D(GLuint textureID);
  void createMultisampleTexture(GLuint textureID);
  void createColorAttachments();

  void createDepthAttachment();

  void deleteFramebuffer();

public:
  /**
   * @brief Constructor for FramebufferGL.
   * Initializes the framebuffer with the specified width, height, number of color attachments, and multisample count.
   * @param width The width of the framebuffer.
   * @param height The height of the framebuffer.
   * @param numColorAttachments The number of color attachments.
   * @param multisampleCount The number of samples for multisampling (default is 1).
   */
  FramebufferGL(int width, int height, int numColorAttachments, int multisampleCount);

  FramebufferGL(const FramebufferGL&)            = delete;
  FramebufferGL& operator=(const FramebufferGL&) = delete;
  FramebufferGL(FramebufferGL&&)                 = delete;
  FramebufferGL& operator=(FramebufferGL&&)      = delete;

  /**
   * @brief Binds the framebuffer for rendering.
   */
  void bind();

  /**
   * @brief Resizes the framebuffer to the specified width and height.
   * @param newWidth The new width of the framebuffer.
   * @param newHeight The new height of the framebuffer.
   */
  void resize(int newWidth, int newHeight);

  /**
   * @brief Sets the texture unit for a specific color attachment.
   * @param index The index of the color attachment (0 to numColorAttachments - 1).
   * @param textureUnit The OpenGL texture unit to bind the color attachment to.
   * @throws std::out_of_range if the index is out of range.
   */
  void setTextureUnit(size_t index, int textureUnit);

  /**
   * @brief Gets the OpenGL ID of the framebuffer.
   * @return The OpenGL ID of the framebuffer.
   */
  GLuint getFramebufferID() const;

  /**
   * @brief Destructor for FramebufferGL.
   */
  ~FramebufferGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_FRAMEBUFFERGL_HPP