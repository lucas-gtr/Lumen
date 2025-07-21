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
  glGenTextures(1, &m_texture_id);
  m_texture_data_observer_id = texture->getTextureDataObserver().add([this]() { uploadToGPU(); });
  m_texture_parameters_observer_id =
      texture->getTextureParametersObserver().add([this, texture]() { configureParameters(texture); });
  std::cout << "TextureGL: Texture created with ID " << m_texture_id << "." << '\n';
}

void TextureGL::bind(int texture_unit) {
  if(texture_unit != 0 && texture_unit != 1) {
    std::cout << "TextureGL: Binding texture with ID " << m_texture_id << " to texture unit " << texture_unit << "."
              << '\n';
  }
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

void TextureGL::configureParameters(const Texture* texture) {
  glActiveTexture(GL_TEXTURE0 + LOAD_TEXTURE_UNIT);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  applyWrappingMode(texture->getWrappingMode(), texture->getBorderColor());
  applyFilteringMode(texture->getFilteringMode());
}

void TextureGL::applyWrappingMode(TextureSampling::TextureWrapping wrapping_mode, const ColorRGB& border_color) {
  const GLint gl_mode = GetGlWrappingMode(wrapping_mode);
  if(gl_mode == -1) {
    return;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_mode);

  if(wrapping_mode == TextureSampling::TextureWrapping::CLAMP_TO_BORDER) {
    std::array<float, 4> gl_border_color = {static_cast<float>(border_color.r), static_cast<float>(border_color.g),
                                            static_cast<float>(border_color.b), 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, gl_border_color.data());
  }
}

void TextureGL::applyFilteringMode(TextureSampling::TextureFiltering filtering_mode) {
  const GLint gl_mode = GetGlFilteringMode(filtering_mode);
  if(gl_mode == -1) {
    return;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mode);
}

GLint TextureGL::GetGlWrappingMode(TextureSampling::TextureWrapping wrapping_mode) {
  switch(wrapping_mode) {
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

GLint TextureGL::GetGlFilteringMode(TextureSampling::TextureFiltering filtering_mode) {
  switch(filtering_mode) {
  case TextureSampling::TextureFiltering::NEAREST:
    return GL_NEAREST;
  case TextureSampling::TextureFiltering::BILINEAR:
    return GL_LINEAR;
  default:
    std::cerr << "Error: Unsupported texture filtering mode." << '\n';
    return -1;
  }
}

GLint TextureGL::GetGlFormat(int channel_count) {
  switch(channel_count) {
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
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  const Texture* texture = getSource();
  configureParameters(texture);

  if(const double* image_data = texture->getImageData()) {
    const auto format = GetGlFormat(texture->getProperties().channels);
    if(format == 0) {
      return;
    }

    const auto [width, height] = std::make_pair(texture->getProperties().width, texture->getProperties().height);
    const int data_size        = width * height * texture->getProperties().channels;

    if(texture->getColorSpace() == ColorSpace::LINEAR) {
      std::vector<float> linear_data(data_size);
      for(size_t i = 0; i < static_cast<size_t>(data_size); ++i) {
        linear_data[i] = static_cast<float>(image_data[i]);
        convertToLinearSpace(linear_data[i]);
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, linear_data.data());
    } else {
      std::vector<unsigned char> converted_data(data_size);
      for(size_t i = 0; i < static_cast<size_t>(data_size); ++i) {
        converted_data[i] = static_cast<unsigned char>(
            std::clamp(image_data[i] * NORMALIZED_TO_COLOR8, 0.0, static_cast<double>(COLOR8_MAX_VALUE)));
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, converted_data.data());
    }
  } else {
    std::cerr << "Error: Texture data is null." << '\n';
  }

  std::cout << "TextureGL: Texture with ID " << m_texture_id << " uploaded to GPU." << '\n';
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureGL::release() {
  if(m_texture_id != 0) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &m_texture_id);
    std::cout << "TextureGL with ID " << m_texture_id << " destroyed." << '\n';

    m_texture_id = 0U;
  }
}

void TextureGL::unbind(int texture_unit) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, 0);
}

TextureGL::~TextureGL() {
  getSource()->getTextureDataObserver().remove(m_texture_data_observer_id);
  getSource()->getTextureParametersObserver().remove(m_texture_parameters_observer_id);
  TextureGL::release();
}

// GCOVR_EXCL_STOP