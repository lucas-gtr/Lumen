#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "Core/Config.hpp"
#include "Rendering/MultiThreadedCPU.hpp"
#include "Rendering/Renderer.hpp"

MultiThreadedCPU::MultiThreadedCPU(int chunk_size, unsigned int thread_count)
    : m_chunk_size(chunk_size), m_thread_count(thread_count) {}

bool MultiThreadedCPU::render() {
  std::cout << m_thread_count << " threads available for rendering." << '\n';

  m_samples_per_pixel = renderer()->getRenderSettings().getSamplesPerPixel();

  const double sample_weight   = 1.0 / m_samples_per_pixel;
  const int    samples_per_row = static_cast<int>(std::sqrt(m_samples_per_pixel));
  const double cell_size       = 1.0 / static_cast<double>(samples_per_row);

  const int height = renderer()->getRenderSettings().getHeight();
  const int width  = renderer()->getRenderSettings().getWidth();

  renderer()->getFramebuffer()->initThreadBuffers(m_thread_count);

  generateChunks(width, height);
  renderer()->getRenderTime()->start(static_cast<int>(m_chunks.size()));

  std::vector<std::thread> threads;
  threads.reserve(m_thread_count);
  for(int t = 0; t < m_thread_count; ++t) {
    threads.emplace_back(&MultiThreadedCPU::threadWorker, this, t, sample_weight, cell_size);
  }

  for(auto& thread : threads) {
    thread.join();
  }

  if(renderer()->isStopRequested()) {
    std::cerr << "Render cancelled by user.\n";
    renderer()->getRenderTime()->stop();
    return false;
  }

  renderer()->getFramebuffer()->reduceThreadBuffers();
  renderer()->getFramebuffer()->clearThreadBuffers();
  renderer()->getRenderTime()->stop();

  return true;
}

void MultiThreadedCPU::generateChunks(int width, int height) {
  m_chunks.clear();

  const int samples_per_row = static_cast<int>(std::sqrt(m_samples_per_pixel));

  for(int s = 0; s < m_samples_per_pixel; ++s) {
    for(int y = 0; y < height; y += m_chunk_size) {
      for(int x = 0; x < width; x += m_chunk_size) {
        Chunk chunk;
        chunk.start             = {x, y};
        chunk.end               = {std::min(x + m_chunk_size, width), std::min(y + m_chunk_size, height)};
        chunk.subpixel_grid_pos = {s % samples_per_row, s / samples_per_row};

        m_chunks.push_back(chunk);
      }
    }
  }
}

void MultiThreadedCPU::threadWorker(int thread_id, double sample_weight, double cell_size) {
  Framebuffer::setThreadId(thread_id);
  while(true) {
    if(renderer()->isStopRequested()) {
      return;
    }
    const int chunk_index = m_current_chunk_index.fetch_add(1);
    if(chunk_index % CHUNK_COUNT_UPDATE_INTERVAL == 0) {
      renderer()->getRenderTime()->update(chunk_index);
    }
    if(chunk_index >= static_cast<int>(m_chunks.size())) {
      break;
    }

    const Chunk& chunk = m_chunks[chunk_index];
    renderer()->renderSample(chunk.start, chunk.end, sample_weight, chunk.subpixel_grid_pos, cell_size);
    if(chunk_index % (m_chunks.size() / m_samples_per_pixel) == 0) {
      renderer()->getRenderProgressObserver().notify(static_cast<double>(chunk_index) /
                                                     static_cast<double>(m_chunks.size()));
    }
  }
}