/**
 * @file MultiThreadedCPU.hpp
 * @brief Header file for the MultiThreadedCPU class.
 */
#ifndef RENDERING_MULTITHREADEDCPU_HPP
#define RENDERING_MULTITHREADEDCPU_HPP

#include <atomic>
#include <vector>

#include "Core/Config.hpp"
#include "Core/ImageTypes.hpp"
#include "Rendering/RenderStrategy.hpp"

/**
 * @struct Chunk
 * @brief Represents a chunk of pixels for rendering.
 */
struct Chunk {
  PixelCoord start;
  PixelCoord end;
  PixelCoord subpixel_grid_pos;
};

/**
 * @class MultiThreadedCPU
 * @brief A rendering strategy that uses multiple threads to render the scene on the CPU.
 *
 * This class divides the rendering task into chunks and assigns each chunk to a separate thread for parallel
 * processing.
 */
class MultiThreadedCPU : public RenderStrategy {
private:
  unsigned int m_thread_count = 1U;

  int                m_samples_per_pixel = 0;
  int                m_chunk_size        = DEFAULT_CHUNK_SIZE;
  std::vector<Chunk> m_chunks;
  std::atomic<int>   m_current_chunk_index{0};

  void generateChunks(int width, int height);
  void threadWorker(int thread_id, double sample_weight, double cell_size);

public:
  /**
   * @brief Default constructor for MultiThreadedCPU.
   * @param chunk_size The size of each chunk in pixels.
   * @param thread_count The number of threads to use for rendering.
   */
  explicit MultiThreadedCPU(int chunk_size, unsigned int thread_count);

  /**
   * @brief Renders the scene using multiple threads.
   * @return True if rendering was successful, false otherwise.
   */
  bool render() override;
};

#endif // RENDERING_MULTITHREADEDCPU_HPP