#include <Eigen/Core>

#include "BVH/BVHNode.hpp"
#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
BVHNode::BVHNode(const Eigen::Vector3d& min_bound, const Eigen::Vector3d& max_bound)
    : m_min_bound(min_bound), m_max_bound(max_bound) {
  m_center = (m_min_bound + m_max_bound) * HALF;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
BVHNode::BVHNode(const Eigen::Vector3d& min_bound, const Eigen::Vector3d& max_bound, int leaf_index)
    : m_min_bound(min_bound), m_max_bound(max_bound), m_leaf_index(leaf_index) {
  m_min_bound -= Eigen::Vector3d::Constant(BVH_CONSTRUCTION_EPSILON);
  m_max_bound += Eigen::Vector3d::Constant(BVH_CONSTRUCTION_EPSILON);
  m_center = (m_min_bound + m_max_bound) * HALF;
}