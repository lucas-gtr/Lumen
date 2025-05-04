#include "Core/Random.hpp"

#include <Eigen/Core>
#include <random>

Eigen::Vector2d randomPointInUnitDisk() {
  static std::mt19937                           gen(std::random_device{}());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  Eigen::Vector2d p = Eigen::Vector2d(dist(gen), dist(gen));
  while(p.squaredNorm() > 1.0) {
    p = Eigen::Vector2d(dist(gen), dist(gen));
  }
  return p;
}

double randomUniform01() {
  static std::mt19937                           gen(std::random_device{}());
  static std::uniform_real_distribution<double> dist(0.0, 1.0);
  return dist(gen);
}
