#include <linalg/Vec3.hpp>

#include "BVH/BVHNode.hpp"
#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"

BVHNode::BVHNode(const linalg::Vec3d& min_bound, const linalg::Vec3d& max_bound)
    : m_min_bound(min_bound), m_max_bound(max_bound) {
  m_center = (m_min_bound + m_max_bound) * HALF;
}

BVHNode::BVHNode(const linalg::Vec3d& min_bound, const linalg::Vec3d& max_bound, int leaf_index)
    : m_min_bound(min_bound), m_max_bound(max_bound), m_leaf_index(leaf_index) {
  m_min_bound -= linalg::Vec3d(BVH_CONSTRUCTION_EPSILON);
  m_max_bound += linalg::Vec3d(BVH_CONSTRUCTION_EPSILON);
  m_center = (m_min_bound + m_max_bound) * HALF;
}