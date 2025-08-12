#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Core/ScopedTimer.hpp"

ScopedTimer::ScopedTimer(const std::string& name) : m_name(name), m_start(std::chrono::high_resolution_clock::now()) {}

void ScopedTimer::stop() {
  if(!m_stopped) {
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count();

    auto& stats = GetStats()[m_name];
    stats.call_count++;
    stats.total_time_microseconds += duration;

    m_stopped = true;
  }
}

ScopedTimer::~ScopedTimer() {
  if(!m_stopped) {
    stop();
  }
}

void ScopedTimer::PrintStats() {
  for(const auto& [name, stat] : GetStats()) {
    std::cout << "[Timer] " << name << " | calls: " << stat.call_count
              << " | total: " << static_cast<double>(stat.total_time_microseconds) / 1000.0 << " ms\n"; // NOLINT
  }
}