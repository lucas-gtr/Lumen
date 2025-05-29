/**
 * @file MultiThreadedCPU.hpp
 * @brief Header file for the MultiThreadedCPU class.
 */
#ifndef RENDERING_MULTITHREADEDCPU_HPP
#define RENDERING_MULTITHREADEDCPU_HPP

#include <atomic>
#include <vector>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "ImplementationParameters/Parameters.hpp"
#include "Rendering/RenderExecution.hpp"

/**
 * @struct Block
 * @brief Represents a block of pixels for rendering.
 */
struct Block {
  PixelCoord start;
  PixelCoord end;
  PixelCoord subpixel_grid_pos;
};

/**
 * @class MultiThreadedCPU
 * @brief A rendering strategy that uses multiple threads to render the scene on the CPU.
 *
 * This class divides the rendering task into blocks and assigns each block to a separate thread for parallel
 * processing.
 */
class MultiThreadedCPU : public RenderStrategy {
private:
  unsigned int m_thread_count = 1U;

  int                m_block_size = DEFAULT_BLOCK_SIZE;
  std::vector<Block> m_blocks;
  std::atomic<int>   m_current_block_index{0};

  void generateBlocks(int width, int height, int samples_per_pixel);
  void threadWorker(int thread_id, double sample_weight, double cell_size);

public:
  explicit MultiThreadedCPU(Parameters* parameters);

  /**
   * @brief Renders the scene using multiple threads.
   * @return True if rendering was successful, false otherwise.
   */
  bool render() override;
};

#endif // RENDERING_MULTITHREADEDCPU_HPP