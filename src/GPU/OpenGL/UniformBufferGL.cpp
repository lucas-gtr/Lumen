// GCOVR_EXCL_START
// NOLINTNEXTLINE(misc-include-cleaner)
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstddef>

#include "GPU/OpenGL/UniformBufferGL.hpp"

UniformBufferGL::UniformBufferGL(size_t size, unsigned int binding_point) : m_size(size) {
  initializeOpenGLFunctions();

  glGenBuffers(1, &m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferGL::updateData(const void* data, size_t size, size_t offset) {
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferGL::~UniformBufferGL() { glDeleteBuffers(1, &m_ubo); }

// GCOVR_EXCL_STOP