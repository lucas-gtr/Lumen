// GCOVR_EXCL_START
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "GPU/OpenGL/FramebufferGL.hpp"

FramebufferGL::FramebufferGL(int width, int height, int num_color_attachments, int multisample_count)
    : m_width(width), m_height(height), m_num_color_attachments(num_color_attachments),
    m_multisample_count(multisample_count) {
  initializeOpenGLFunctions();
  createFramebuffer();
}

void FramebufferGL::resize(int new_width, int new_height) {
  m_width  = new_width;
  m_height = new_height;
  deleteFramebuffer();
  createFramebuffer();
}

void FramebufferGL::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_id);
  glViewport(0, 0, m_width, m_height);
}

void FramebufferGL::setTextureUnit(size_t index, int texture_unit) {
  if(index >= m_color_attachments.size()) {
    throw std::out_of_range("Color attachment index out of range");
  }
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  const GLenum target = (m_multisample_count > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  glBindTexture(target, m_color_attachments[index]);
}

GLuint FramebufferGL::getFramebufferID() const { return m_framebuffer_id; }

void FramebufferGL::createFramebuffer() {
  glGenFramebuffers(1, &m_framebuffer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_id);

  createColorAttachments();
  createDepthAttachment();

  std::vector<GLenum> draw_buffers(m_num_color_attachments);
  for(int i = 0; i < m_num_color_attachments; ++i) {
    draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  glDrawBuffers(static_cast<GLsizei>(draw_buffers.size()), draw_buffers.data());

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer is not complete");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferGL::createTexture2D(GLuint texture_id) {
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void FramebufferGL::createMultisampleTexture(GLuint texture_id) {
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_id);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_multisample_count, GL_RGBA16F, m_width, m_height, GL_TRUE);
}

void FramebufferGL::createColorAttachments() {
  m_color_attachments.resize(m_num_color_attachments);
  glGenTextures(m_num_color_attachments, m_color_attachments.data());
  for(int i = 0; i < m_num_color_attachments; ++i) {
    if(m_multisample_count > 1) {
      createMultisampleTexture(m_color_attachments[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, m_color_attachments[i],
                             0);
    } else {
      createTexture2D(m_color_attachments[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_color_attachments[i], 0);
    }
  }
}

void FramebufferGL::createDepthAttachment() {
  glGenRenderbuffers(1, &m_depth_renderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depth_renderbuffer);

  if(m_multisample_count > 1) {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_multisample_count, GL_DEPTH24_STENCIL8, m_width, m_height);
  } else {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_renderbuffer);
}

void FramebufferGL::deleteFramebuffer() {
  if(!m_color_attachments.empty()) {
    glDeleteTextures(static_cast<GLsizei>(m_color_attachments.size()), m_color_attachments.data());
    m_color_attachments.clear();
  }

  if(m_depth_renderbuffer != 0U) {
    glDeleteRenderbuffers(1, &m_depth_renderbuffer);
    m_depth_renderbuffer = 0;
  }

  if(m_framebuffer_id != 0U) {
    glDeleteFramebuffers(1, &m_framebuffer_id);
    m_framebuffer_id = 0;
  }
}

FramebufferGL::~FramebufferGL() { deleteFramebuffer(); }

// GCOVR_EXCL_STOP