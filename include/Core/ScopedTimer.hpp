/**
 * @file ScopedTimer.hpp
 * @brief Header file for the ScopedTimer class, which measures the execution time of code blocks.
 */
#ifndef CORE_SCOPEDTIMER_HPP
#define CORE_SCOPEDTIMER_HPP

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @class ScopedTimer
 * @brief Class that measures the execution time of a code block.
 *
 * This class is used to measure the time taken by a specific section of code.
 * It automatically starts the timer when an instance is created and stops it
 * when the instance goes out of scope, printing the elapsed time.
 */
class ScopedTimer {
private:
  struct Stat {
    int64_t call_count              = 0;
    int64_t total_time_microseconds = 0;
  };

  bool m_stopped = false;

  const std::string                                    m_name;
  const std::chrono::high_resolution_clock::time_point m_start;

  static std::unordered_map<std::string, Stat>& GetStats() {
    static std::unordered_map<std::string, Stat> stats;
    return stats;
  }

public:
  /**
   * @brief Constructor that starts the timer for the given name.
   * @param name The name of the code block being timed.
   */
  explicit ScopedTimer(const std::string& name);

  ScopedTimer(const ScopedTimer&)            = delete;
  ScopedTimer& operator=(const ScopedTimer&) = delete;
  ScopedTimer(ScopedTimer&&)                 = delete;
  ScopedTimer& operator=(ScopedTimer&&)      = delete;

  void stop(); ///< Stops the timer and records the elapsed time.

  static void PrintStats(); ///< Prints the statistics of all timed code blocks.

  ~ScopedTimer(); ///< Destructor that stops the timer if it hasn't been stopped already.
};

#endif // CORE_SCOPEDTIMER_HPP