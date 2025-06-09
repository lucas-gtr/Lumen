#include <random>

#include "Core/Math/Vec2.hpp"
#include "Core/Random.hpp"

lin::Vec2d randomPointInUnitDisk() {
  static std::mt19937                           gen(std::random_device{}());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  lin::Vec2d p = {dist(gen), dist(gen)};
  while(p.squaredLength() > 1.0) {
    p = {dist(gen), dist(gen)};
  }
  return p;
}

double randomUniform01() {
  static std::mt19937                           gen(std::random_device{}());
  static std::uniform_real_distribution<double> dist(0.0, 1.0);
  return dist(gen);
}
