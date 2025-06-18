#include <memory>

#include "Core/Config.hpp"
#include "GPU/OpenGL/SkyboxGL.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Qt/OpenGLContext.hpp"
#include "Scene/Skybox.hpp"

SkyboxGL::SkyboxGL(Skybox* skybox) : m_skybox(skybox) {
  updateSkyboxTexture();
  m_skybox->getTextureObserver().add([this]() { updateSkyboxTexture(); });
}

void SkyboxGL::updateSkyboxTexture() {
  if(m_skybox->getTexture() != nullptr) {
    OpenGLContext::instance().makeContextCurrent();
    m_texture = std::make_unique<TextureGL>(m_skybox->getTexture());
    m_texture->uploadToGPU();
    m_texture->bind(SKYBOX_TEXTURE_UNIT);
    OpenGLContext::instance().doneContext();
  }
}

TextureGL* SkyboxGL::getTexture() const { return m_texture.get(); }