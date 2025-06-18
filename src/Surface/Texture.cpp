#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>
#include <vector>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
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
  generateTexture({1, 1, 1}, {value});
  finalizeTexture();
}

void Texture::setValue(const ColorRGB& color) {
  generateTexture({1, 1, 3}, {color.r, color.g, color.b});
  finalizeTexture();
}

void Texture::setValue(const ColorRGBA& color) {
  generateTexture({1, 1, 4}, {color.r, color.g, color.b, color.a});
  finalizeTexture();
}

void Texture::setTextureType(TextureType type) {
  if(type == TextureType::COLOR_TEXTURE) {
    generateTexture({1, 1, 3}, {1.0, 1.0, 1.0});
  } else {
    generateTexture({1, 1, 3}, {1.0, 0.0, 1.0});
  }

  m_texture_type = type;
  finalizeTexture();
}

void Texture::loadFromFile(const char* filename) {
  TextureLoader::load(filename, m_image_data, m_texture_properties);
  m_texture_path = filename;
  finalizeTexture();
}

void Texture::generateTexture(const ImageProperties& properties, const std::vector<double>& image_data) {
  m_texture_properties = properties;
  m_image_data         = image_data;
  m_texture_path.clear();

  finalizeTexture();
}

void Texture::finalizeTexture() {
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
  m_texture_data_observer.notify();
}

void Texture::setFlippedVertically(bool flipped) {
  if(flipped != m_flipped_vertically) {
    m_flipped_vertically = flipped;
    flipVertically();
  }
}

double Texture::getValue1d(TextureUV uv_coord) const {
  if(m_texture_properties.channels == 3) {
    return toGrayscale(getValue3d(uv_coord));
  }
  if(m_texture_properties.channels == 4) {
    return toGrayscale(getValue4d(uv_coord));
  }
  TextureSampling::wrapCoordinates(uv_coord, m_wrapping_mode);

  double     value     = 0.0;
  const bool is_border = (uv_coord.u == -1.0 || uv_coord.v == -1.0);

  if(is_border) {
    value = toGrayscale(m_border_color);
  } else {
    const int w = m_texture_properties.width;
    const int h = m_texture_properties.height;
    const int c = m_texture_properties.channels;

    switch(m_filtering_mode) {
    case TextureSampling::TextureFiltering::NEAREST: {
      const auto coord = TextureSampling::sampleNearest(uv_coord, {w, h});
      value            = m_image_data[coord.y * w * c + coord.x * c];
      break;
    }
    case TextureSampling::TextureFiltering::BILINEAR: {
      const auto   info = TextureSampling::sampleBilinear(uv_coord, {w, h});
      auto         get  = [&](int x, int y) { return m_image_data[y * w * c + x * c]; };
      const double c00  = get(info.x0, info.y0);
      const double c01  = get(info.x1, info.y0);
      const double c10  = get(info.x0, info.y1);
      const double c11  = get(info.x1, info.y1);
      value             = (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 +
              (1.0 - info.dx) * info.dy * c10 + info.dx * info.dy * c11;
      break;
    }
    }
  }

  if(m_color_space == ColorSpace::Linear) {
    convertToLinearSpace(value);
  }

  return value;
}

ColorRGB Texture::getValue3d(TextureUV uv_coord) const {
  if(m_texture_properties.channels == 1) {
    return toRGB(getValue1d(uv_coord));
  }
  if(m_texture_properties.channels == 4) {
    return toRGB(getValue4d(uv_coord));
  }

  TextureSampling::wrapCoordinates(uv_coord, m_wrapping_mode);

  ColorRGB   color;
  const bool is_border = (uv_coord.u == -1.0 || uv_coord.v == -1.0);

  if(is_border) {
    color = ColorRGB(m_border_color);
  } else {
    const int w = m_texture_properties.width;
    const int h = m_texture_properties.height;
    const int c = m_texture_properties.channels;

    switch(m_filtering_mode) {
    case TextureSampling::TextureFiltering::NEAREST: {
      const auto coord = TextureSampling::sampleNearest(uv_coord, {w, h});
      const int  index = coord.y * w * c + coord.x * c;
      color            = {m_image_data[index + 0], m_image_data[index + 1], m_image_data[index + 2]};
      break;
    }
    case TextureSampling::TextureFiltering::BILINEAR: {
      const auto info = TextureSampling::sampleBilinear(uv_coord, {w, h});
      auto       get  = [&](int x, int y) -> ColorRGB {
        const int idx = y * w * c + x * c;
        return {m_image_data[idx + 0], m_image_data[idx + 1], m_image_data[idx + 2]};
      };
      const ColorRGB c00 = get(info.x0, info.y0);
      const ColorRGB c01 = get(info.x1, info.y0);
      const ColorRGB c10 = get(info.x0, info.y1);
      const ColorRGB c11 = get(info.x1, info.y1);
      color              = (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 +
              (1.0 - info.dx) * info.dy * c10 + info.dx * info.dy * c11;
      break;
    }
    }
  }

  if(m_color_space == ColorSpace::Linear) {
    convertToLinearSpace(color);
  }
  return color;
}

ColorRGBA Texture::getValue4d(TextureUV uv_coord) const {
  if(m_texture_properties.channels == 1) {
    return toRGBA(getValue1d(uv_coord));
  }
  if(m_texture_properties.channels == 3) {
    return toRGBA(getValue3d(uv_coord));
  }
  TextureSampling::wrapCoordinates(uv_coord, m_wrapping_mode);

  ColorRGBA  color;
  const bool is_border = (uv_coord.u == -1.0 || uv_coord.v == -1.0);

  if(is_border) {
    color = m_border_color;
  } else {
    const int w = m_texture_properties.width;
    const int h = m_texture_properties.height;
    const int c = m_texture_properties.channels;

    switch(m_filtering_mode) {
    case TextureSampling::TextureFiltering::NEAREST: {
      const auto coord = TextureSampling::sampleNearest(uv_coord, {w, h});
      const int  index = coord.y * w * c + coord.x * c;
      color = {m_image_data[index + 0], m_image_data[index + 1], m_image_data[index + 2], m_image_data[index + 3]};
      break;
    }
    case TextureSampling::TextureFiltering::BILINEAR: {
      const auto info = TextureSampling::sampleBilinear(uv_coord, {w, h});
      auto       get  = [&](int x, int y) -> ColorRGBA {
        const int idx = y * w * c + x * c;
        return {m_image_data[idx + 0], m_image_data[idx + 1], m_image_data[idx + 2], m_image_data[idx + 3]};
      };
      const ColorRGBA c00 = get(info.x0, info.y0);
      const ColorRGBA c01 = get(info.x1, info.y0);
      const ColorRGBA c10 = get(info.x0, info.y1);
      const ColorRGBA c11 = get(info.x1, info.y1);
      color               = (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 +
              (1.0 - info.dx) * info.dy * c10 + info.dx * info.dy * c11;
      break;
    }
    }
  }

  if(m_color_space == ColorSpace::Linear) {
    convertToLinearSpace(color);
  }
  return color;
}

void Texture::setColorSpace(ColorSpace colorSpace) {
  m_color_space = colorSpace;
  m_texture_data_observer.notify();
}

void Texture::setBorderColor(const ColorRGBA& color) {
  m_border_color = color;
  m_texture_parameters_observer.notify();
}

void Texture::setBorderColor(const ColorRGB& color) {
  m_border_color = ColorRGBA(color, 1.0);
  m_texture_parameters_observer.notify();
}

void Texture::setBorderColor(double value) {
  m_border_color = {value, value, value, 1.0};
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