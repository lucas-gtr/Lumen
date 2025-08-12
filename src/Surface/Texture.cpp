#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>
#include <vector>

#include "Core/Color.hpp"
#include "Core/Config.hpp"
#include "Core/ImageTypes.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureFiltering.hpp"
#include "Surface/TextureLoader.hpp"
#include "Surface/TextureWrapping.hpp"

Texture::Texture()
    : m_texture_properties({1, 1, 3}), m_image_data({1.0, 0.0, 1.0}), m_preview_properties({1, 1, 3}),
      m_texture_preview({COLOR8_MAX_VALUE, 0, COLOR8_MAX_VALUE}) {
  generatePreviewData();
}

void Texture::generatePreviewData() {
  int new_width                   = 0;
  int new_height                  = 0;
  std::tie(new_width, new_height) = computePreviewSize();

  m_texture_preview.clear();
  m_texture_preview.reserve(static_cast<std::size_t>(new_width) * static_cast<std::size_t>(new_height) *
                            static_cast<std::size_t>(m_texture_properties.channels));
  m_preview_properties = {new_width, new_height, m_texture_properties.channels};

  generateRescaledImage(new_width, new_height);
}

std::pair<int, int> Texture::computePreviewSize() const {
  const int original_width  = m_texture_properties.width;
  const int original_height = m_texture_properties.height;

  if(original_width <= MAX_PREVIEW_TEXTURE_SIZE && original_height <= MAX_PREVIEW_TEXTURE_SIZE) {
    return {original_width, original_height};
  }
  if(original_width > original_height) {
    const int resized_height =
        static_cast<int>(original_height * (static_cast<double>(MAX_PREVIEW_TEXTURE_SIZE) / original_width));
    return {MAX_PREVIEW_TEXTURE_SIZE, resized_height};
  }

  const int resized_width =
      static_cast<int>(original_width * (static_cast<double>(MAX_PREVIEW_TEXTURE_SIZE) / original_height));
  return {resized_width, MAX_PREVIEW_TEXTURE_SIZE};
}

void Texture::generateRescaledImage(int new_width, int new_height) {
  const int original_width  = m_texture_properties.width;
  const int original_height = m_texture_properties.height;
  const int channels        = m_texture_properties.channels;

  for(int y = 0; y < new_height; ++y) {
    for(int x = 0; x < new_width; ++x) {
      const double u = static_cast<double>(x) / new_width;
      const double v = static_cast<double>(y) / new_height;

      const int src_x = static_cast<int>(u * original_width);
      const int src_y = static_cast<int>(v * original_height);

      appendPixelFromSource(src_x, src_y, channels, original_width);
    }
  }
}

void Texture::appendPixelFromSource(int src_x, int src_y, int channels, int original_width) {
  for(int c = 0; c < channels; ++c) {
    const std::uint64_t src_index = (static_cast<std::uint64_t>(src_y) * original_width + src_x) * channels + c;
    m_texture_preview.push_back(static_cast<unsigned char>(m_image_data[src_index] * NORMALIZED_TO_COLOR8));
  }
}

void Texture::setValue(double value) {
  m_texture_type = TextureType::COLOR_TEXTURE;
  generateTexture({1, 1, 1}, {value});
}

void Texture::setValue(const ColorRGB& color) {
  m_texture_type = TextureType::COLOR_TEXTURE;
  generateTexture({1, 1, 3}, {color.r, color.g, color.b});
}

void Texture::setValue(const ColorRGBA& color) {
  m_texture_type = TextureType::COLOR_TEXTURE;
  generateTexture({1, 1, 4}, {color.r, color.g, color.b, color.a});
}

void Texture::setTextureType(TextureType type) {
  if(type == TextureType::COLOR_TEXTURE) {
    generateTexture({1, 1, 3}, {1.0, 1.0, 1.0});
  } else {
    generateTexture({1, 1, 3}, {1.0, 0.0, 1.0});
  }

  m_texture_type = type;
  generatePreviewData();
  m_texture_data_observer.notify();
}

void Texture::loadFromFile(const char* filename) {
  TextureLoader::load(filename, m_image_data, m_texture_properties);
  m_texture_type = TextureType::IMAGE_TEXTURE;
  m_texture_path = filename;
  generatePreviewData();
  m_texture_data_observer.notify();
}

void Texture::generateTexture(const ImageProperties& properties, const std::vector<double>& image_data) {
  m_texture_properties = properties;
  m_image_data         = image_data;
  m_texture_path.clear();

  generatePreviewData();
  m_texture_data_observer.notify();
}

void Texture::flipVertically() {
  for(int y = 0; y < m_texture_properties.height / 2; ++y) {
    for(int x = 0; x < m_texture_properties.width; ++x) {
      for(int c = 0; c < m_texture_properties.channels; ++c) {
        std::swap(m_image_data[(y * m_texture_properties.width + x) * m_texture_properties.channels + c],
                  m_image_data[((m_texture_properties.height - 1 - y) * m_texture_properties.width + x) *
                                   m_texture_properties.channels +
                               c]);
      }
    }
  }
  generatePreviewData();
  m_texture_data_observer.notify();
}

void Texture::setFlippedVertically(bool flipped) {
  if(flipped != m_flipped_vertically) {
    m_flipped_vertically = flipped;
    flipVertically();
  }
}

void Texture::readPixelChannels(int x, int y, double* out) const {
  const int w     = m_texture_properties.width;
  const int c     = m_texture_properties.channels;
  const int index = (y * w + x) * c;
  for(int i = 0; i < c; ++i) {
    out[i] = m_image_data[index + i];
  }
}

ColorRGBA Texture::samplePixelColor(TextureUV uv) const {
  TextureSampling::wrapCoordinates(uv, m_wrapping_mode);
  const int             c = m_texture_properties.channels;
  std::array<double, 4> out{0.0, 0.0, 0.0, 1.0};

  if(uv.u == -1.0 || uv.v == -1.0) {
    if(c == 1) {
      const double g = m_border_color.grayscale();
      out[0] = out[1] = out[2] = g;
    } else {
      out[0] = m_border_color.r;
      out[1] = m_border_color.g;
      out[2] = m_border_color.b;
    }
  } else {
    const int w = m_texture_properties.width;
    const int h = m_texture_properties.height;

    if(m_filtering_mode == TextureSampling::TextureFiltering::NEAREST) {
      const auto coord = TextureSampling::sampleNearest(uv, {w, h});
      readPixelChannels(coord.x, coord.y, out.data());
    } else {
      const auto            info = TextureSampling::sampleBilinear(uv, {w, h});
      std::array<double, 4> c00{}, c01{}, c10{}, c11{}; // NOLINT(readability-isolate-declaration)
      readPixelChannels(info.x0, info.y0, c00.data());
      readPixelChannels(info.x1, info.y0, c01.data());
      readPixelChannels(info.x0, info.y1, c10.data());
      readPixelChannels(info.x1, info.y1, c11.data());

      for(int ch = 0; ch < c; ++ch) {
        out[ch] = (1.0 - info.dx) * (1.0 - info.dy) * c00[ch] + info.dx * (1.0 - info.dy) * c01[ch] +
                  (1.0 - info.dx) * info.dy * c10[ch] + info.dx * info.dy * c11[ch];
      }
    }
  }

  ColorRGBA color{0.0};
  switch(c) {
  case 1:
    color = {out[0], out[0], out[0], 1.0};
    break;
  case 3:
    color = {out[0], out[1], out[2], 1.0};
    break;
  default:
    color = {out[0], out[1], out[2], out[3]};
    break;
  }

  if(m_color_space == ColorSpace::LINEAR) {
    color = color.toLinearSpace();
  }
  return color;
}

double Texture::getValue1d(TextureUV uv) const {
  const ColorRGBA c = samplePixelColor(uv);
  return c.grayscale();
}

ColorRGB Texture::getValue3d(TextureUV uv) const {
  const ColorRGBA c = samplePixelColor(uv);
  return {c.r, c.g, c.b};
}

ColorRGBA Texture::getValue4d(TextureUV uv) const { return samplePixelColor(uv); }

void Texture::setColorSpace(ColorSpace color_space) {
  m_color_space = color_space;
  m_texture_parameters_observer.notify();
}

void Texture::setBorderColor(const ColorRGBA& color) {
  m_border_color = ColorRGB(color);
  m_texture_parameters_observer.notify();
}

void Texture::setBorderColor(const ColorRGB& color) {
  m_border_color = color;
  m_texture_parameters_observer.notify();
}

void Texture::setBorderColor(double value) {
  m_border_color = {value, value, value};
  m_texture_parameters_observer.notify();
}

void Texture::setFilteringMode(TextureSampling::TextureFiltering filtering) {
  m_filtering_mode = filtering;
  m_texture_parameters_observer.notify();
}

void Texture::setWrappingMode(TextureSampling::TextureWrapping wrapping) {
  m_wrapping_mode = wrapping;
  m_texture_parameters_observer.notify();
}