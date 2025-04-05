#include <climits>
#include <eigen/Eigen/Core>
#include <iostream>

int main() {
  const Eigen::Matrix3f mat = Eigen::Matrix3f::Identity();
  std::cout << "Identity matrix:\n" << mat << '\n';

  return 0;
}