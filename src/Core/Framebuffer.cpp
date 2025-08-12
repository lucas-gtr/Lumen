#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

#include "Core/Color.hpp"
#include "Core/Framebuffer.hpp"
#include "Core/ImageTypes.hpp"

thread_local int Framebuffer::m_thread_id = -1;

Framebuffer::Framebuffer(Resolution resolution) : m_resolution(resolution) { updateFrameBuffer(); }

void Framebuffer::updateFrameBuffer() {
  delete[] m_framebuffer;
  m_buffer_size = m_buffer_size = static_cast<size_t>(m_resolution.width) * m_resolution.height * m_channel_count;
  m_framebuffer                 = new double[m_buffer_size];
  std::fill_n(m_framebuffer, m_buffer_size, 0.0);
}

void Framebuffer::setResolution(Resolution resolution) {
  if(m_resolution.width != resolution.width || m_resolution.height != resolution.height) {
    m_resolution = resolution;
    updateFrameBuffer();
  }
}

void Framebuffer::convertToSRGBColorSpace() {
  for(int i = 0; i < m_resolution.width * m_resolution.height; ++i) {
    for(int j = 0; j < 3; j++) {
      convertToSRGBSpace(m_framebuffer[m_channel_count * i + j]);
    }
  }
}

void Framebuffer::initThreadBuffers(unsigned int num_threads) {
  num_threads = std::max(1U, num_threads);
  m_thread_buffers.resize(num_threads);
  for(auto& buffer : m_thread_buffers) {
    buffer.resize(m_buffer_size, 0.0);
  }
}

void Framebuffer::reduceThreadBuffers() {
  std::vector<int> indices(m_buffer_size);
  std::iota(indices.begin(), indices.end(), 0);

  std::for_each(std::execution::par, indices.begin(), indices.end(), [&](int index) {
    const double sum = std::transform_reduce(std::execution::seq, m_thread_buffers.begin(), m_thread_buffers.end(), 0.0,
                                             std::plus<>(), [=](const auto& buffer) { return buffer[index]; });
    m_framebuffer[index] = sum;
  });
}

void Framebuffer::scaleBufferValues(double factor) {
  for(size_t i = 0; i < m_buffer_size; ++i) {
    m_framebuffer[i] *= factor;
  }
}

void Framebuffer::clearThreadBuffers() { m_thread_buffers = std::vector<std::vector<double>>(); }

void Framebuffer::setPixelColor(const PixelCoord& pixel_coord, const ColorRGB& color, double weight) {
  if(pixel_coord.x < 0 || pixel_coord.x >= m_resolution.width || pixel_coord.y < 0 ||
     pixel_coord.y >= m_resolution.height) {
    std::cerr << "Pixel coordinates out of bounds: (" << pixel_coord.x << ", " << pixel_coord.y << ").\n";
    return;
  }

  if(m_thread_id < 0 || m_thread_id >= static_cast<int>(m_thread_buffers.size())) {
    std::cerr << "Invalid thread ID: " << m_thread_id << ".\n";
    return;
  }

  const int index = m_channel_count * (pixel_coord.y * m_resolution.width + pixel_coord.x);

  m_thread_buffers[m_thread_id][index] += color.r * weight;
  m_thread_buffers[m_thread_id][index + 1] += color.g * weight;
  m_thread_buffers[m_thread_id][index + 2] += color.b * weight;
}

double Framebuffer::getMaximumValue() const {
  double max_value = 0.0;
  for(size_t i = 0; i < m_buffer_size; ++i) {
    if(m_channel_count == 4 && i % 4 == 3) {
      continue; // Skip alpha channel
    }
    max_value = std::max(max_value, m_framebuffer[i]);
  }
  return max_value;
}

Framebuffer::~Framebuffer() { delete[] m_framebuffer; }
