#include <algorithm>
#include <chrono>

#include "Rendering/RenderTime.hpp"

void RenderTime::start(int total_chunks) {
  m_total_chunks  = total_chunks;
  m_current_chunk = 0;
  m_start_time    = std::chrono::steady_clock::now();
  m_is_running    = true;
}

void RenderTime::stop() { m_is_running = false; }

void RenderTime::update(int current_chunk) {
  if(current_chunk <= m_total_chunks) {
    m_current_chunk = current_chunk;
  }
  if(m_is_running) {
    const double elapsed  = std::chrono::duration<double>(std::chrono::steady_clock::now() - m_start_time).count();
    const double progress = static_cast<double>(m_current_chunk) / m_total_chunks;
    if(progress > 0.0) {
      const double total_estimated = elapsed / progress;
      m_estimated_end_time         = m_start_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                                                std::chrono::duration<double>(total_estimated));
    }
  }
}

RenderStats RenderTime::getRenderStats() const {
  RenderStats stats;

  auto now                = std::chrono::steady_clock::now();
  auto remaining_duration = m_estimated_end_time - now;

  stats.elapsed_time   = std::chrono::duration<double>(now - m_start_time).count();
  stats.remaining_time = std::max(0.0, std::chrono::duration<double>(remaining_duration).count());

  stats.current_chunk = m_current_chunk;
  stats.total_chunks  = m_total_chunks;

  return stats;
}
