#ifndef RENDERING_RENDERTIME_HPP
#define RENDERING_RENDERTIME_HPP

#include <chrono>

struct RenderStats {
  double elapsed_time   = 0.0;
  double remaining_time = 0.0;
  int    current_chunk  = 0;
  int    total_chunks   = 0;
};

class RenderTime {
public:
  RenderTime() = default;

  void start(int total_chunks);
  void stop();
  void update(int current_chunk);

  RenderStats getRenderStats() const;

private:
  std::chrono::steady_clock::time_point m_start_time;
  std::chrono::steady_clock::time_point m_estimated_end_time; // estimation du temps de fin
  bool                                  m_is_running = false;

  int m_total_chunks  = 0;
  int m_current_chunk = 0;
};

#endif // RENDERING_RENDERTIME_HPP
