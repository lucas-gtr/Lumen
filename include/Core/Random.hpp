/**
 * @file Random.hpp
 * @brief Header file for random number generation functions.
 */
#ifndef CORE_RANDOM_HPP
#define CORE_RANDOM_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <linalg/Vec2.hpp>
#include <linalg/Vec3.hpp>
#include <thread>

#include "Core/MathConstants.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
inline double intToDouble(uint64_t i) {
  i = (UINT64_C(0x3FF) << 52U) | (i >> 12U);
  double d{};
  std::memcpy(&d, &i, sizeof(d));
  return d - 1.0;
}

inline double randomUniform01() {
  const thread_local uint64_t thread_seed = std::hash<std::thread::id>()(std::this_thread::get_id());

  thread_local std::array<uint64_t, 4> shuffle_table = {
      0x1234567890abcdefU ^ thread_seed, 0xfedcba0987654321U ^ thread_seed, 0x1122334455667788U ^ thread_seed,
      0x8877665544332211U ^ thread_seed};

  const uint64_t result_plus = shuffle_table[0] + shuffle_table[3];

  const uint64_t t = shuffle_table[1] << 17U;

  shuffle_table[2] ^= shuffle_table[0];
  shuffle_table[3] ^= shuffle_table[1];
  shuffle_table[1] ^= shuffle_table[2];
  shuffle_table[0] ^= shuffle_table[3];

  shuffle_table[2] ^= t;

  shuffle_table[3] = (shuffle_table[3] << 45U) | (shuffle_table[3] >> 19U);

  return intToDouble(result_plus);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

inline linalg::Vec2d randomPointInUnitDisk() {
  const double r     = std::sqrt(randomUniform01());
  const double theta = randomUniform01() * 2 * PI;
  return {r * std::cos(theta), r * std::sin(theta)};
}

inline linalg::Vec3d randomPointOnUnitSphere() {
  const double u = randomUniform01();
  const double v = randomUniform01();

  const double theta = 2 * PI * u;
  const double phi   = std::acos(1.0 - 2 * v);

  const double x = std::sin(phi) * std::cos(theta);
  const double y = std::sin(phi) * std::sin(theta);
  const double z = std::cos(phi);

  return {x, y, z};
}

#endif // CORE_RANDOM_HPP