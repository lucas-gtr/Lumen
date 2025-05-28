#include "Core/Framebuffer.hpp"
#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>

Framebuffer::Framebuffer(ImageProperties properties) : m_framebuffer_properties(properties) { updateFrameBuffer(); }

void Framebuffer::updateFrameBuffer() {
  delete[] m_framebuffer;
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  m_framebuffer = new double[m_framebuffer_properties.bufferSize()];
  for(std::uint64_t i = 0; i < m_framebuffer_properties.bufferSize(); ++i) {
    m_framebuffer[i] = 0.0;
  }
}

void Framebuffer::setFramebufferProperties(ImageProperties properties) {
  if(m_framebuffer_properties.width != properties.width || m_framebuffer_properties.height != properties.height ||
     m_framebuffer_properties.channels != properties.channels) {
    m_framebuffer_properties = properties;
    updateFrameBuffer();
  }
}

void Framebuffer::convertToSRGBColorSpace() {
  for(int i = 0; i < m_framebuffer_properties.width * m_framebuffer_properties.height; ++i) {
    for(int j = 0; j < std::min(m_framebuffer_properties.channels, 3); j++) {
      convertToSRGBSpace(m_framebuffer[i * m_framebuffer_properties.channels + j]);
    }
  }
}

void Framebuffer::setPixelColor(PixelCoord pixel_coord, const ColorRGBA& color, double weight) {
  if(pixel_coord.x < 0 || pixel_coord.x >= m_framebuffer_properties.width || pixel_coord.y < 0 ||
     pixel_coord.y >= m_framebuffer_properties.height) {
    std::cerr << "Pixel coordinates out of bounds: (" << pixel_coord.x << ", " << pixel_coord.y << ").\n";
    return;
  }
  const int index =
      (pixel_coord.y * m_framebuffer_properties.width + pixel_coord.x) * m_framebuffer_properties.channels;

  switch(m_framebuffer_properties.channels) {
  case 1:
    m_framebuffer[index] += toGrayscale(color) * weight;
    break;
  case 3:
    m_framebuffer[index] += color.r * weight;
    m_framebuffer[index + 1] += color.g * weight;
    m_framebuffer[index + 2] += color.b * weight;
    break;
  case 4:
    m_framebuffer[index] += color.r * weight;
    m_framebuffer[index + 1] += color.g * weight;
    m_framebuffer[index + 2] += color.b * weight;
    m_framebuffer[index + 3] += color.a * weight;
    break;
  default:
    std::cerr << "Unsupported channel count: " << m_framebuffer_properties.channels
              << ". Supported counts are 1, 3, or 4." << '\n';
    break; // Unsupported channel count
  }
}

Framebuffer::~Framebuffer() { delete[] m_framebuffer; }
