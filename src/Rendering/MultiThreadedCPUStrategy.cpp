#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "Core/Config.hpp"
#include "ImplementationParameters/ParameterTypes.hpp"
#include "ImplementationParameters/Parameters.hpp"
#include "Rendering/MultiThreadedCPU.hpp"
#include "Rendering/Renderer.hpp"

MultiThreadedCPU::MultiThreadedCPU(Parameters* parameters) {
  parameters->setDefaultParameter("thread_count",
                                  static_cast<int>(std::max(1U, std::thread::hardware_concurrency() - 2)));
  parameters->setDefaultParameter("block_size", m_block_size);

  const IntParam* thread_count_param = parameters->get<IntParam>("thread_count");
  if(thread_count_param != nullptr) {
    m_thread_count = std::clamp(static_cast<unsigned int>(thread_count_param->getValue()), 1U,
                                std::thread::hardware_concurrency() - 2);
  }

  const IntParam* block_size_param = parameters->get<IntParam>("block_size");
  if(block_size_param != nullptr) {
    m_block_size = std::clamp(block_size_param->getValue(), MIN_BLOCK_SIZE, MAX_BLOCK_SIZE);
  }
}

bool MultiThreadedCPU::render() {
  std::cout << m_thread_count << " threads available for rendering." << '\n';

  const int samples_per_pixel = getRenderer()->getRenderSettings().getSamplesPerPixel();

  const double sample_weight   = 1.0 / samples_per_pixel;
  const int    samples_per_row = static_cast<int>(std::sqrt(samples_per_pixel));
  const double cell_size       = 1.0 / static_cast<double>(samples_per_row);

  const int height = getRenderer()->getRenderSettings().getHeight();
  const int width  = getRenderer()->getRenderSettings().getWidth();

  getRenderer()->getFramebuffer()->initThreadBuffers(m_thread_count);

  generateBlocks(width, height, samples_per_pixel);

  std::vector<std::thread> threads;
  threads.reserve(m_thread_count);
  for(int t = 0; t < m_thread_count; ++t) {
    threads.emplace_back(&MultiThreadedCPU::threadWorker, this, t, sample_weight, cell_size);
  }

  for(auto& thread : threads) {
    thread.join();
  }

  getRenderer()->getFramebuffer()->reduceThreadBuffers();

  return true;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void MultiThreadedCPU::generateBlocks(int width, int height, int samples_per_pixel) {
  m_blocks.clear();

  const int samples_per_row = static_cast<int>(std::sqrt(samples_per_pixel));

  for(int s = 0; s < samples_per_pixel; ++s) {
    for(int y = 0; y < height; y += m_block_size) {
      for(int x = 0; x < width; x += m_block_size) {
        Block block;
        block.start             = {x, y};
        block.end               = {std::min(x + m_block_size, width), std::min(y + m_block_size, height)};
        block.subpixel_grid_pos = {s % samples_per_row, s / samples_per_row};

        m_blocks.push_back(block);
      }
    }
  }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void MultiThreadedCPU::threadWorker(int thread_id, double sample_weight, double cell_size) {
  Framebuffer::setThreadId(thread_id);
  while(true) {
    const int block_index = m_current_block_index.fetch_add(1);
    if(block_index >= static_cast<int>(m_blocks.size())) {
      break;
    }

    if(block_index % static_cast<int>(static_cast<double>(m_blocks.size()) * sample_weight) == 0) {
      std::cout << block_index << " / " << m_blocks.size() << " blocks rendered." << '\n';
    }
    const Block& block = m_blocks[block_index];
    getRenderer()->renderSample(block.start, block.end, sample_weight, block.subpixel_grid_pos, cell_size);
  }
}