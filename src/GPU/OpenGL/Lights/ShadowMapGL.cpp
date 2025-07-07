// GCOVR_EXCL_START
#include <GL/gl.h>
#include <GL/glext.h>
#include <algorithm>
#include <array>
#include <stdexcept>

#include "Core/Config.hpp"
#include "GPU/OpenGL/Lights/ShadowMapGL.hpp"

ShadowMapGL::ShadowMapGL(int size) : m_size(size) { initializeOpenGLFunctions(); }

void ShadowMapGL::createFramebuffer() {
  glGenFramebuffers(1, &m_depth_map_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map_fbo);
}

void ShadowMapGL::createDepthTexture2D() {
  glGenTextures(1, &m_depth_map);
  glActiveTexture(GL_TEXTURE0 + LOAD_TEXTURE_UNIT);
  glBindTexture(GL_TEXTURE_2D, m_depth_map);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_size, m_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  std::array<float, 4> border_color = {1.0F, 1.0F, 1.0F, 1.0F};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color.data());
}

void ShadowMapGL::createDepthTextureCube() {
  glGenTextures(1, &m_depth_map);
  glActiveTexture(GL_TEXTURE0 + LOAD_TEXTURE_UNIT);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_depth_map);

  for(unsigned int i = 0; i < CUBE_MAP_FACE_COUNT; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32, m_size, m_size, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, nullptr);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void ShadowMapGL::attachDepthTexture(GLenum target) {
  if(target == GL_TEXTURE_CUBE_MAP) {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_map, 0);
  } else {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, m_depth_map, 0);
  }
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
}

void ShadowMapGL::validateFramebuffer() {
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Shadow map framebuffer is not complete!");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapGL::initialize2DMap() {
  m_depth_map_type = GL_TEXTURE_2D;

  createFramebuffer();
  createDepthTexture2D();
  attachDepthTexture(GL_TEXTURE_2D);
  validateFramebuffer();
}

void ShadowMapGL::initializeCubeMap() {
  m_depth_map_type = GL_TEXTURE_CUBE_MAP;

  createFramebuffer();
  createDepthTextureCube();
  attachDepthTexture(GL_TEXTURE_CUBE_MAP);
  validateFramebuffer();
}

void ShadowMapGL::resize(int new_size) {
  cleanup();
  m_size = std::clamp(new_size, MIN_SHADOW_MAP_SIZE, MAX_SHADOW_MAP_SIZE);
  if(m_depth_map_type == GL_TEXTURE_2D) {
    initialize2DMap();

  } else {
    initializeCubeMap();
  }
}

void ShadowMapGL::bindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map_fbo); }

void ShadowMapGL::bindTexture(unsigned int texture_unit) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(m_depth_map_type, m_depth_map);
}

void ShadowMapGL::cleanup() {
  if(m_depth_map_fbo != 0) {
    glDeleteFramebuffers(1, &m_depth_map_fbo);
    m_depth_map_fbo = 0;
  }
  if(m_depth_map != 0) {
    glDeleteTextures(1, &m_depth_map);
    m_depth_map = 0;
  }
}

ShadowMapGL::~ShadowMapGL() { cleanup(); }

// GCOVR_EXCL_STOP