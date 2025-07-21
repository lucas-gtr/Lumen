// GCOVR_EXCL_START
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

#include "Core/Config.hpp"
#include "GPU/IObjectGPU.hpp"
#include "GPU/OpenGL/MaterialGL.hpp"
#include "GPU/OpenGL/ObjectGL.hpp"
#include "SceneObjects/Object3D.hpp"

ObjectGL::ObjectGL(Object3D* object, MaterialGL* material) : IObjectGPU(object), m_material(material) {
  initializeOpenGLFunctions();

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  std::cout << "ObjectGL: VAO " << m_vao << " created." << '\n';
}

void ObjectGL::setMaterial(MaterialGL* material) {
  if(m_material != material) {
    m_material = material;
    std::cout << "ObjectGL: Material changed for VAO " << m_vao << "." << '\n';
  }
}

void ObjectGL::uploadVertices() {
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(dataSize()), vertices().data(), GL_STATIC_DRAW);
}

void ObjectGL::uploadIndices() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indicesSize()), indices().data(), GL_STATIC_DRAW);
}

// NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting, performance-no-int-to-ptr)
void ObjectGL::setupVertexAttributes() {

  // position (x,y,z)
  glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)(POSITION_OFFSET));
  glEnableVertexAttribArray(POSITION_LOCATION);

  // normal (x,y,z)
  glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)(NORMAL_OFFSET));
  glEnableVertexAttribArray(NORMAL_LOCATION);

  // uv (u,v)
  glVertexAttribPointer(UV_LOCATION, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)(UV_OFFSET));
  glEnableVertexAttribArray(UV_LOCATION);

  // tangent (x,y,z)
  glVertexAttribPointer(TANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)(TANGENT_OFFSET));
  glEnableVertexAttribArray(TANGENT_LOCATION);

  // bitangent (x,y,z)
  glVertexAttribPointer(BITANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)(BITANGENT_OFFSET));
  glEnableVertexAttribArray(BITANGENT_LOCATION);
}
// NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting, performance-no-int-to-ptr)

void ObjectGL::uploadToGPU() {
  glBindVertexArray(m_vao);

  uploadVertices();
  uploadIndices();
  setupVertexAttributes();

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  std::cout << "ObjectGL: Data uploaded to GPU to VAO " << m_vao << "." << '\n';
}

void ObjectGL::bindVAO() { glBindVertexArray(m_vao); }

void ObjectGL::bindMaterial() { m_material->bind(); }

void ObjectGL::unbind() {
  glBindVertexArray(0);
  m_material->unbind();
}

void ObjectGL::release() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(m_vao != 0U) {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }
  if(m_vbo != 0U) {
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
  }
  if(m_ebo != 0U) {
    glDeleteBuffers(1, &m_ebo);
    m_ebo = 0;
  }
}

ObjectGL::~ObjectGL() {
  ObjectGL::release();
  std::cout << "ObjectGL with VAO " << m_vao << " destroyed." << '\n';
}

// GCOVR_EXCL_STOP