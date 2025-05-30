#include <algorithm>
#include <memory>
#include <vector>

#include "BVH/BVHBuilder.hpp"
#include "BVH/BVHNode.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"

namespace BVH {
int getLargestAxis(const lin::Vec3& extent) {
  if(extent.y > extent.x && extent.y > extent.z) {
    return 1;
  }

  if(extent.z > extent.x) {
    return 2;
  }

  return 0;
}

void constructNode(std::shared_ptr<BVHNode>& node, std::vector<std::shared_ptr<BVHNode>>& objects, int start, int end) {
  if(start >= end) {
    return;
  }
  if(end - start == 1) {
    node = objects[start];
    return;
  }

  lin::Vec3 min_bound = objects[start]->getMinBound();
  lin::Vec3 max_bound = objects[start]->getMaxBound();
  for(int i = start + 1; i < end; ++i) {
    min_bound = lin::cwiseMin(min_bound, objects[i]->getMinBound());
    max_bound = lin::cwiseMax(max_bound, objects[i]->getMaxBound());
  }
  node = std::make_shared<BVHNode>(min_bound, max_bound);

  const lin::Vec3 extent = max_bound - min_bound;
  const int       axis   = getLargestAxis(extent);

  const int mid = (start + end) / 2;
  std::nth_element(objects.begin() + start, objects.begin() + mid, objects.begin() + end,
                   [axis](const std::shared_ptr<BVHNode>& a, const std::shared_ptr<BVHNode>& b) {
                     return a->getCenter()[axis] < b->getCenter()[axis];
                   });

  // Recursively construct child nodes
  constructNode(node->getLeftChild(), objects, start, mid);
  constructNode(node->getRightChild(), objects, mid, end);
}
}; // namespace BVH