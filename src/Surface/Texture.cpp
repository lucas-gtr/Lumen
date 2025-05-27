#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <utility>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureFiltering.hpp"
#include "Surface/TextureWrapping.hpp"

Texture::Texture(double value) : m_texture_properties({1, 1, 1}), m_image_data(new double[1]) {
  m_image_data[0] = value;
}

Texture::Texture(const ColorRGB& color) : m_texture_properties({1, 1, 3}), m_image_data(new double[3]) {
  m_image_data[0] = color.r;
  m_image_data[1] = color.g;
  m_image_data[2] = color.b;
}

Texture::Texture(const ColorRGBA& color) : m_texture_properties({1, 1, 4}), m_image_data(new double[4]) {
  m_image_data[0] = color.r;
  m_image_data[1] = color.g;
  m_image_data[2] = color.b;
  m_image_data[3] = color.a;
}

Texture::Texture(const unsigned char* image_data, ImageProperties texture_properties)
    : m_texture_properties(texture_properties), m_image_data(new double[texture_properties.bufferSize()]) {
  for(std::uint64_t i = 0; i < texture_properties.bufferSize(); ++i) {
    m_image_data[i] = static_cast<double>(image_data[i]) * COLOR8_TO_NORMALIZED;
  }
}

Texture::Texture(const double* image_data, ImageProperties texture_properties)
    : m_texture_properties(texture_properties), m_image_data(new double[texture_properties.bufferSize()]) {
  for(std::uint64_t i = 0; i < texture_properties.bufferSize(); ++i) {
    m_image_data[i] = image_data[i];
  }
}

Texture::Texture(const Texture& other)
    : m_texture_properties(other.m_texture_properties), m_wrapping_mode(other.m_wrapping_mode),
      m_filtering_mode(other.m_filtering_mode), m_border_color(other.m_border_color),
      m_image_data(new double[other.m_texture_properties.bufferSize()]) {
  std::copy(other.m_image_data, other.m_image_data + static_cast<ptrdiff_t>(m_texture_properties.bufferSize()),
            m_image_data);
}

Texture::Texture(Texture&& other) noexcept
    : m_texture_properties(other.m_texture_properties), m_wrapping_mode(other.m_wrapping_mode),
      m_filtering_mode(other.m_filtering_mode), m_border_color(other.m_border_color), m_image_data(other.m_image_data) {
  other.m_image_data = nullptr;
}

Texture& Texture::operator=(const Texture& other) {
  if(this != &other) {
    delete[] m_image_data;

    m_texture_properties = other.m_texture_properties;
    m_wrapping_mode      = other.m_wrapping_mode;
    m_filtering_mode     = other.m_filtering_mode;
    m_border_color       = other.m_border_color;

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    m_image_data = new double[m_texture_properties.bufferSize()];
    std::copy(other.m_image_data, other.m_image_data + static_cast<ptrdiff_t>(m_texture_properties.bufferSize()),
              m_image_data);
  }
  return *this;
}

Texture& Texture::operator=(Texture&& other) noexcept {
  if(this != &other) {
    delete[] m_image_data;

    m_texture_properties = other.m_texture_properties;
    m_wrapping_mode      = other.m_wrapping_mode;
    m_filtering_mode     = other.m_filtering_mode;
    m_border_color       = other.m_border_color;

    m_image_data       = other.m_image_data;
    other.m_image_data = nullptr;
  }
  return *this;
}

void Texture::setValue(double value) {
  delete[] m_image_data;

  m_texture_properties = {1, 1, 1};
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  m_image_data    = new double[1];
  m_image_data[0] = value;
}

void Texture::setValue(const ColorRGB& color) {
  delete[] m_image_data;

  m_texture_properties = {1, 1, 3};
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  m_image_data    = new double[3];
  m_image_data[0] = color.r;
  m_image_data[1] = color.g;
  m_image_data[2] = color.b;
}

void Texture::setValue(const ColorRGBA& color) {
  delete[] m_image_data;

  m_texture_properties = {1, 1, 4};
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  m_image_data    = new double[4];
  m_image_data[0] = color.r;
  m_image_data[1] = color.g;
  m_image_data[2] = color.b;
  m_image_data[3] = color.a;
}

void Texture::setImageData(const double* image_data, ImageProperties texture_properties) {
  delete[] m_image_data;

  m_texture_properties = texture_properties;
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  m_image_data = new double[m_texture_properties.bufferSize()];
  for(std::uint64_t i = 0; i < m_texture_properties.bufferSize(); ++i) {
    m_image_data[i] = image_data[i];
  }
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
}

// Eigen::VectorXd Texture::getValueNd(TextureUV uv_coord, int channels) const {
//   switch(m_filtering_mode) {
//     case TextureSampling::TextureFiltering::NEAREST: {
//       auto pixel_coord =
//           TextureSampling::sampleNearest(uv_coord, {m_texture_properties.width, m_texture_properties.height});
//       Eigen::VectorXd result(channels);
//       for(int c = 0; c < channels; ++c) {
//         result[c] = m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
//                                 pixel_coord.x * m_texture_properties.channels + c];
//       }
//       return result;
//     }
//     case TextureSampling::TextureFiltering::BILINEAR: {
//       const auto info =
//           TextureSampling::sampleBilinear(uv_coord, {m_texture_properties.width, m_texture_properties.height});
//       Eigen::VectorXd c00(channels);
//       Eigen::VectorXd c01(channels);
//       Eigen::VectorXd c10(channels);
//       Eigen::VectorXd c11(channels);

//       for(int c = 0; c < channels; ++c) {
//         c00[c] = m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
//                               info.x0 * m_texture_properties.channels + c];
//         c01[c] = m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
//                               info.x1 * m_texture_properties.channels + c];
//         c10[c] = m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
//                               info.x0 * m_texture_properties.channels + c];
//         c11[c] = m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
//                               info.x1 * m_texture_properties.channels + c];
//       }
//       return (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 + (1.0 - info.dx) * info.dy * c10 +
//             info.dx * info.dy * c11;
//     }
//   }

//   return Eigen::VectorXd::Zero(channels);
// }

double Texture::getValue1d(TextureUV uv_coord) const {
  if(m_texture_properties.channels == 3) {
    return toGrayscale(getValue3d(uv_coord));
  }
  if(m_texture_properties.channels == 4) {
    return toGrayscale(getValue4d(uv_coord));
  }

  TextureSampling::wrapCoordinates(uv_coord, m_wrapping_mode);
  if(uv_coord.u == -1.0 || uv_coord.v == -1.0) {
    return toGrayscale(m_border_color);
  }

  switch(m_filtering_mode) {
    case TextureSampling::TextureFiltering::NEAREST: {
      auto pixel_coord =
          TextureSampling::sampleNearest(uv_coord, {m_texture_properties.width, m_texture_properties.height});
      return m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                          pixel_coord.x * m_texture_properties.channels];
      }
    case TextureSampling::TextureFiltering::BILINEAR: {
      const auto info =
          TextureSampling::sampleBilinear(uv_coord, {m_texture_properties.width, m_texture_properties.height});
      double c00 = m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels];
      double c01 = m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels];
      double c10 = m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels];
      double c11 = m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels];
      return (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 + (1.0 - info.dx) * info.dy * c10 +
             info.dx * info.dy * c11;
    }
  }
}

ColorRGB Texture::getValue3d(TextureUV uv_coord) const {
  if(m_texture_properties.channels == 1) {
    return toRGB(getValue1d(uv_coord));
  }
  if(m_texture_properties.channels == 4) {
    return toRGB(getValue4d(uv_coord));
  }

  TextureSampling::wrapCoordinates(uv_coord, m_wrapping_mode);
  if(uv_coord.u == -1.0 || uv_coord.v == -1.0) {
    return ColorRGB(m_border_color);
  }

  switch(m_filtering_mode) {
    case TextureSampling::TextureFiltering::NEAREST: {
      auto pixel_coord =
          TextureSampling::sampleNearest(uv_coord, {m_texture_properties.width, m_texture_properties.height});
      return ColorRGB(m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 0],
                      m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 1],
                      m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 2]);
    }
    case TextureSampling::TextureFiltering::BILINEAR: {
      const auto info =
          TextureSampling::sampleBilinear(uv_coord, {m_texture_properties.width, m_texture_properties.height});
      ColorRGB c00(m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 0],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 1],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 2]);
      ColorRGB c01(m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 0],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 1],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 2]);
      ColorRGB c10(m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 0],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 1],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 2]);
      ColorRGB c11(m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 0],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 1],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 2]);
      return (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 + (1.0 - info.dx) * info.dy * c10 +
             info.dx * info.dy * c11;
    }
  }
}

ColorRGBA Texture::getValue4d(TextureUV uv_coord) const {
  if(m_texture_properties.channels == 1) {
    return toRGBA(getValue1d(uv_coord));
  }
  if(m_texture_properties.channels == 3) {
    return toRGBA(getValue3d(uv_coord));
  }

  TextureSampling::wrapCoordinates(uv_coord, m_wrapping_mode);
  if(uv_coord.u == -1.0 || uv_coord.v == -1.0) {
    return m_border_color;
  }

  switch(m_filtering_mode) {
    case TextureSampling::TextureFiltering::NEAREST: {
      auto pixel_coord =
          TextureSampling::sampleNearest(uv_coord, {m_texture_properties.width, m_texture_properties.height});
      return ColorRGBA(m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 0],
                      m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 1],
                      m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 2],
                      m_image_data[pixel_coord.y * m_texture_properties.width * m_texture_properties.channels +
                                   pixel_coord.x * m_texture_properties.channels + 3]);
    }
    case TextureSampling::TextureFiltering::BILINEAR: {
      const auto info =
          TextureSampling::sampleBilinear(uv_coord, {m_texture_properties.width, m_texture_properties.height});
      ColorRGBA c00(m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 0],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 1],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 2],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 3]);
      ColorRGBA c01(m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 0],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 1],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 2],
                   m_image_data[info.y0 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 3]);
      ColorRGBA c10(m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 0],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 1],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 2],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x0 * m_texture_properties.channels + 3]);
      ColorRGBA c11(m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 0],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 1],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 2],
                   m_image_data[info.y1 * m_texture_properties.width * m_texture_properties.channels +
                                 info.x1 * m_texture_properties.channels + 3]);
      return (1.0 - info.dx) * (1.0 - info.dy) * c00 + info.dx * (1.0 - info.dy) * c01 + (1.0 - info.dx) * info.dy * c10 +
             info.dx * info.dy * c11;
    }
  }
}

void Texture::setColorSpace(ColorSpace colorSpace) {
  if(colorSpace == m_color_space) {
    return;
  }
  m_color_space = colorSpace;

  for(int i = 0; i < m_texture_properties.width * m_texture_properties.height; ++i) {
    for(int j = 0; j < std::min(m_texture_properties.channels, 3); ++j) {
      double& value = m_image_data[i * m_texture_properties.channels + j];
      if(m_color_space == ColorSpace::RGB) {
        convertToLinearSpace(value);
      } else if(m_color_space == ColorSpace::sRGB) {
        convertToSRGBSpace(value);
      }
    }
  }
}

Texture::~Texture() { delete[] m_image_data; }