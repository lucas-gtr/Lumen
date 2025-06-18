// GCOVR_EXCL_START
#include "GPU/OpenGL/MaterialGL.hpp"
#include "Core/Config.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Surface/Material.hpp"

MaterialGL::MaterialGL(Material* material, TextureGL* diffuse_texture, TextureGL* normal_texture)
    : m_material(material), m_diffuse_texture(diffuse_texture), m_normal_texture(normal_texture) {
  initializeOpenGLFunctions();
}

void MaterialGL::bind() {
  if(m_diffuse_texture != nullptr) {
    m_diffuse_texture->bind(DIFFUSE_TEXTURE_UNIT);
  }
  if(m_normal_texture != nullptr) {
    m_normal_texture->bind(NORMAL_TEXTURE_UNIT);
  }
}

void MaterialGL::unbind() {
  m_diffuse_texture->unbind(DIFFUSE_TEXTURE_UNIT);
  m_normal_texture->unbind(NORMAL_TEXTURE_UNIT);
}

// GCOVR_EXCL_STOP
