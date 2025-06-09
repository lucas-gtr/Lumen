// GCOVR_EXCL_START
#include <GL/glew.h>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "GPU/OpenGL/FramebufferGL.hpp"

FramebufferGL::FramebufferGL(int width, int height, int numColorAttachments, int multisampleCount)
    : m_width(width), m_height(height), m_numColorAttachments(numColorAttachments),
      m_multisampleCount(multisampleCount) {
  createFramebuffer();
}

void FramebufferGL::resize(int newWidth, int newHeight) {
  m_width  = newWidth;
  m_height = newHeight;
  deleteFramebuffer();
  createFramebuffer();
}

void FramebufferGL::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
  glViewport(0, 0, m_width, m_height);
}

void FramebufferGL::setTextureUnit(size_t index, int textureUnit) {
  if(index >= m_colorAttachments.size()) {
    throw std::out_of_range("Color attachment index out of range");
  }

  glActiveTexture(GL_TEXTURE0 + textureUnit);
  const GLenum target = (m_multisampleCount > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  glBindTexture(target, m_colorAttachments[index]);
}

GLuint FramebufferGL::getFramebufferID() const { return m_framebufferID; }

void FramebufferGL::createFramebuffer() {
  glGenFramebuffers(1, &m_framebufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

  createColorAttachments();
  createDepthAttachment();

  std::vector<GLenum> drawBuffers(m_numColorAttachments);
  for(int i = 0; i < m_numColorAttachments; ++i) {
    drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer is not complete");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferGL::createTexture2D(GLuint textureID) const {
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void FramebufferGL::createMultisampleTexture(GLuint textureID) const {
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_multisampleCount, GL_RGBA16F, m_width, m_height, GL_TRUE);
}

void FramebufferGL::createColorAttachments() {
  m_colorAttachments.resize(m_numColorAttachments);
  glGenTextures(m_numColorAttachments, m_colorAttachments.data());

  for(int i = 0; i < m_numColorAttachments; ++i) {
    if(m_multisampleCount > 1) {
      createMultisampleTexture(m_colorAttachments[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachments[i],
                             0);
    } else {
      createTexture2D(m_colorAttachments[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorAttachments[i], 0);
    }
  }
}

void FramebufferGL::createDepthAttachment() {
  glGenRenderbuffers(1, &m_depthRenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);

  if(m_multisampleCount > 1) {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_multisampleCount, GL_DEPTH24_STENCIL8, m_width, m_height);
  } else {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderbuffer);
}

void FramebufferGL::deleteFramebuffer() {
  if(!m_colorAttachments.empty()) {
    glDeleteTextures(static_cast<GLsizei>(m_colorAttachments.size()), m_colorAttachments.data());
    m_colorAttachments.clear();
  }

  if(m_depthRenderbuffer != 0U) {
    glDeleteRenderbuffers(1, &m_depthRenderbuffer);
    m_depthRenderbuffer = 0;
  }

  if(m_framebufferID != 0U) {
    glDeleteFramebuffers(1, &m_framebufferID);
    m_framebufferID = 0;
  }
}

FramebufferGL::~FramebufferGL() { deleteFramebuffer(); }

// GCOVR_EXCL_STOP