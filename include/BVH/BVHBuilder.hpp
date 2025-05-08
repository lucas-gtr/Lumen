#ifndef BVH_BVHBUILDER_HPP
#define BVH_BVHBUILDER_HPP

#include <memory>
#include <vector>

#include "BVH/BVHNode.hpp"

namespace BVH {
  int getLargestAxis(const Eigen::Vector3d& extent);
  void constructNode(std::shared_ptr<BVHNode>& node, std::vector<std::shared_ptr<BVHNode>>& objects, int start, int end);
}; // namespace BVH


#endif // BVH_BVHBUILDER_HPP