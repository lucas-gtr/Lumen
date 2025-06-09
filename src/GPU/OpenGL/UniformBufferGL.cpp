// GCOVR_EXCL_START
#include <GL/glew.h>
#include <cstddef>

#include "GPU/OpenGL/UniformBufferGL.hpp"

UniformBufferGL::UniformBufferGL(size_t size, unsigned int bindingPoint) : m_size(size) {
  glGenBuffers(1, &m_UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
  glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferGL::updateData(const void* data, size_t size, size_t offset) const {
  glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
  glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferGL::~UniformBufferGL() { glDeleteBuffers(1, &m_UBO); }

// GCOVR_EXCL_STOP