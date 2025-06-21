#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"

thread_local int Framebuffer::m_thread_id = -1;

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

void Framebuffer::initThreadBuffers(unsigned int num_threads) {
  num_threads = std::clamp(num_threads, 1U, std::thread::hardware_concurrency() - 4);
  std::cout << "Initializing thread buffers for " << num_threads << " threads.\n";
  m_thread_buffers.resize(num_threads);
  for(auto& buffer : m_thread_buffers) {
    buffer.resize(m_framebuffer_properties.bufferSize(), 0.0);
  }
}

void Framebuffer::reduceThreadBuffers() {
  const int pixel_count = m_framebuffer_properties.width * m_framebuffer_properties.height;
  const int channels    = m_framebuffer_properties.channels;

  std::vector<int> indices(static_cast<size_t>(pixel_count * channels));
  std::iota(indices.begin(), indices.end(), 0);

  std::for_each(std::execution::par, indices.begin(), indices.end(), [&](int index) {
    const double sum = std::transform_reduce(std::execution::seq, m_thread_buffers.begin(), m_thread_buffers.end(), 0.0,
                                             std::plus<>(), [=](const auto& buffer) { return buffer[index]; });
    m_framebuffer[index] = sum;
  });
}

void Framebuffer::scaleBufferValues(double factor) {
  if(factor <= 0.0) {
    std::cerr << "Factor must be greater than zero. No changes applied.\n";
    return;
  }

  for(std::uint64_t i = 0; i < m_framebuffer_properties.bufferSize(); ++i) {
    m_framebuffer[i] *= factor;
  }
}

void Framebuffer::clearThreadBuffers() { m_thread_buffers = std::vector<std::vector<double>>(); }

void Framebuffer::setPixelColor(const PixelCoord& pixel_coord, const ColorRGBA& color, double weight) {
  if(pixel_coord.x < 0 || pixel_coord.x >= m_framebuffer_properties.width || pixel_coord.y < 0 ||
     pixel_coord.y >= m_framebuffer_properties.height) {
    std::cerr << "Pixel coordinates out of bounds: (" << pixel_coord.x << ", " << pixel_coord.y << ").\n";
    return;
  }

  if(m_thread_id < 0 || m_thread_id >= static_cast<int>(m_thread_buffers.size())) {
    std::cerr << "Invalid thread ID: " << m_thread_id << ".\n";
    return;
  }

  const int index =
      (pixel_coord.y * m_framebuffer_properties.width + pixel_coord.x) * m_framebuffer_properties.channels;

  switch(m_framebuffer_properties.channels) {
  case 1:
    m_thread_buffers[m_thread_id][index] += toGrayscale(color) * weight;
    break;
  case 3:
    m_thread_buffers[m_thread_id][index] += color.r * weight;
    m_thread_buffers[m_thread_id][index + 1] += color.g * weight;
    m_thread_buffers[m_thread_id][index + 2] += color.b * weight;
    break;
  case 4:
    m_thread_buffers[m_thread_id][index] += color.r * weight;
    m_thread_buffers[m_thread_id][index + 1] += color.g * weight;
    m_thread_buffers[m_thread_id][index + 2] += color.b * weight;
    m_thread_buffers[m_thread_id][index + 3] += 1.0 * weight;
    break;
  default:
    std::cerr << "Unsupported channel count: " << m_framebuffer_properties.channels
              << ". Supported counts are 1, 3, or 4." << '\n';
    break; // Unsupported channel count
  }
}

Framebuffer::~Framebuffer() { delete[] m_framebuffer; }
