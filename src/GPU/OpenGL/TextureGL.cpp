// GCOVR_EXCL_START
#include <GL/gl.h>
#include <GL/glext.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "GPU/ITextureGPU.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureFiltering.hpp"
#include "Surface/TextureWrapping.hpp"

TextureGL::TextureGL(Texture* texture) : ITextureGPU(texture) {
  initializeOpenGLFunctions();
  glGenTextures(1, &m_textureID);
  m_texture_data_observer_id = texture->getTextureDataObserver().add([this]() { uploadToGPU(); });
  m_texture_parameters_observer_id =
      texture->getTextureParametersObserver().add([this, texture]() { configureParameters(texture); });
  std::cout << "TextureGL: Texture created with ID " << m_textureID << "." << '\n';
}

void TextureGL::bind(int textureUnit) {
  if(textureUnit != 0 && textureUnit != 1) {
    std::cout << "TextureGL: Binding texture with ID " << m_textureID << " to texture unit " << textureUnit << "."
              << '\n';
  }
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void TextureGL::configureParameters(const Texture* texture) {
  glActiveTexture(GL_TEXTURE0 + LOAD_TEXTURE_UNIT);
  glBindTexture(GL_TEXTURE_2D, m_textureID);

  applyWrappingMode(texture->getWrappingMode(), texture->getBorderColor());
  applyFilteringMode(texture->getFilteringMode());
}

void TextureGL::applyWrappingMode(TextureSampling::TextureWrapping wrappingMode, const ColorRGBA& borderColor) {
  const GLint glMode = getGLWrappingMode(wrappingMode);
  if(glMode == -1) {
    return;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glMode);

  if(wrappingMode == TextureSampling::TextureWrapping::CLAMP_TO_BORDER) {
    std::array<float, 4> glBorderColor = {static_cast<float>(borderColor.r), static_cast<float>(borderColor.g),
                                          static_cast<float>(borderColor.b), static_cast<float>(borderColor.a)};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glBorderColor.data());
  }
}

void TextureGL::applyFilteringMode(TextureSampling::TextureFiltering filteringMode) {
  const GLint glMode = getGLFilteringMode(filteringMode);
  if(glMode == -1) {
    return;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMode);
}

GLint TextureGL::getGLWrappingMode(TextureSampling::TextureWrapping wrappingMode) {
  switch(wrappingMode) {
  case TextureSampling::TextureWrapping::REPEAT:
    return GL_REPEAT;
  case TextureSampling::TextureWrapping::MIRRORED_REPEAT:
    return GL_MIRRORED_REPEAT;
  case TextureSampling::TextureWrapping::CLAMP_TO_EDGE:
    return GL_CLAMP_TO_EDGE;
  case TextureSampling::TextureWrapping::CLAMP_TO_BORDER:
    return GL_CLAMP_TO_BORDER;
  default:
    std::cerr << "Error: Unsupported texture wrapping mode." << '\n';
    return -1;
  }
}

GLint TextureGL::getGLFilteringMode(TextureSampling::TextureFiltering filteringMode) {
  switch(filteringMode) {
  case TextureSampling::TextureFiltering::NEAREST:
    return GL_NEAREST;
  case TextureSampling::TextureFiltering::BILINEAR:
    return GL_LINEAR;
  default:
    std::cerr << "Error: Unsupported texture filtering mode." << '\n';
    return -1;
  }
}

GLint TextureGL::getGLFormat(int channelCount) {
  switch(channelCount) {
  case 1:
    return GL_RED;
  case 3:
    return GL_RGB;
  case 4:
    return GL_RGBA;
  default:
    std::cerr << "Error: Unsupported number of channels in texture." << '\n';
    return 0;
  }
}
void TextureGL::uploadToGPU() {
  glActiveTexture(GL_TEXTURE0 + LOAD_TEXTURE_UNIT);
  glBindTexture(GL_TEXTURE_2D, m_textureID);

  const Texture* texture = getSource();
  configureParameters(texture);

  if(const double* imageData = texture->getImageData()) {
    const auto format = getGLFormat(texture->getProperties().channels);
    if(format == 0) {
      return;
    }

    const auto [width, height] = std::make_pair(texture->getProperties().width, texture->getProperties().height);
    const int dataSize         = width * height * texture->getProperties().channels;

    if(texture->getColorSpace() == ColorSpace::Linear) {
      std::vector<float> linearData(dataSize);
      for(size_t i = 0; i < static_cast<size_t>(dataSize); ++i) {
        linearData[i] = static_cast<float>(imageData[i]);
        convertToLinearSpace(linearData[i]);
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, linearData.data());
    } else {
      std::vector<unsigned char> convertedData(dataSize);
      for(size_t i = 0; i < static_cast<size_t>(dataSize); ++i) {
        convertedData[i] = static_cast<unsigned char>(
            std::clamp(imageData[i] * NORMALIZED_TO_COLOR8, 0.0, static_cast<double>(COLOR8_MAX_VALUE)));
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, convertedData.data());
    }
  } else {
    std::cerr << "Error: Texture data is null." << '\n';
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureGL::release() {
  if(m_textureID != 0) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &m_textureID);
    std::cout << "TextureGL with ID " << m_textureID << " destroyed." << '\n';

    m_textureID = 0U;
  }
}

void TextureGL::unbind(int textureUnit) {
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, 0);
}

TextureGL::~TextureGL() {
  getSource()->getTextureDataObserver().remove(m_texture_data_observer_id);
  getSource()->getTextureParametersObserver().remove(m_texture_parameters_observer_id);
  TextureGL::release();
}

// GCOVR_EXCL_STOP