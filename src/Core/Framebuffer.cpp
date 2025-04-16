#include "Core/Framebuffer.hpp"
#include "Core/MathConstants.hpp"

#include <Eigen/Core>
#include <cstddef>
#include <cstdint>

Framebuffer::Framebuffer(ImageProperties properties)
    : m_width(properties.width), m_height(properties.height), m_channel_count(properties.channel_count) {
  updateFrameBuffer();
}

void Framebuffer::updateFrameBuffer() {
  m_framebuffer.clear();
  m_framebuffer.resize(m_width * m_height * m_channel_count, 0.0);
}

void Framebuffer::setFramebufferProperties(ImageProperties properties) {
  if(m_width != properties.width || m_height != properties.height || m_channel_count != properties.channel_count) {
    m_width         = properties.width;
    m_height        = properties.height;
    m_channel_count = properties.channel_count;
    updateFrameBuffer();
  }
}

void Framebuffer::generateImage() {
  m_image.clear();
  m_image.resize(m_width * m_height * m_channel_count, 0.0);

  for(size_t i = 0; i < m_width * m_height * m_channel_count; ++i) {
    m_image[i] = static_cast<unsigned char>(m_framebuffer[i] * NORMALIZED_TO_COLOR8);
  }
}

void Framebuffer::setPixelColor(size_t x, size_t y, const Eigen::Vector3d& color, double weight) {
  if(x < 0 || x >= m_width || y < 0 || y >= m_height) {
    return;
  }
  const size_t index = (y * m_width + x) * m_channel_count;
  for(size_t c = 0; c < m_channel_count; ++c) {
    m_framebuffer[index + c] += color[static_cast<int64_t>(c)] * weight;
  }
}