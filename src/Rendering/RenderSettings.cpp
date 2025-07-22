#include <algorithm>
#include <cmath>
#include <thread>

#include "Core/Config.hpp"
#include "Rendering/RenderSettings.hpp"

void RenderSettings::setSamplesPerPixel(int samples_per_pixels) {
  samples_per_pixels = std::clamp(samples_per_pixels, MIN_SAMPLES_PER_PIXEL, MAX_SAMPLES_PER_PIXEL);

  const int sqrt_value   = static_cast<int>(std::sqrt(samples_per_pixels));
  const int lower_square = sqrt_value * sqrt_value;
  const int upper_square = (sqrt_value + 1) * (sqrt_value + 1);

  if(samples_per_pixels - lower_square <= upper_square - samples_per_pixels) {
    m_samples_per_pixels = std::max(1, lower_square);
  } else {
    m_samples_per_pixels = std::max(1, upper_square);
  }
}

void RenderSettings::setChunkSize(int chunk_size) {
  m_chunk_size = std::clamp(chunk_size, MIN_CHUNK_SIZE, MAX_CHUNK_SIZE);
}

void RenderSettings::setThreadCount(unsigned int thread_count) {
  m_thread_count = std::clamp(thread_count, 1U, std::max(1U, std::thread::hardware_concurrency() - THREADS_TO_KEEP_FREE));
}