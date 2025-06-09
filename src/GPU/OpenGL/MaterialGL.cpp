// GCOVR_EXCL_START

#include <GL/glew.h>

#include "Core/Config.hpp"
#include "GPU/OpenGL/MaterialGL.hpp"
#include "GPU/OpenGL/TextureGL.hpp"

void MaterialGL::bind() const {
  if(m_albedoTexture != nullptr) {
    m_albedoTexture->bind(DIFFUSE_TEXTURE_UNIT);
  }
  if(m_normalTexture != nullptr) {
    m_normalTexture->bind(NORMAL_TEXTURE_UNIT);
  }
}

void MaterialGL::unbind() {
  TextureGL::unbind(DIFFUSE_TEXTURE_UNIT);
  TextureGL::unbind(NORMAL_TEXTURE_UNIT);
}

// GCOVR_EXCL_STOP
