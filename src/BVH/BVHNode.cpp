#include "BVH/BVHNode.hpp"
#include "Core/Config.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/MathConstants.hpp"

BVHNode::BVHNode(const lin::Vec3& min_bound, const lin::Vec3& max_bound)
    : m_min_bound(min_bound), m_max_bound(max_bound) {
  m_center = (m_min_bound + m_max_bound) * HALF;
}

BVHNode::BVHNode(const lin::Vec3& min_bound, const lin::Vec3& max_bound, int leaf_index)
    : m_min_bound(min_bound), m_max_bound(max_bound), m_leaf_index(leaf_index) {
  m_min_bound -= lin::Vec3(BVH_CONSTRUCTION_EPSILON);
  m_max_bound += lin::Vec3(BVH_CONSTRUCTION_EPSILON);
  m_center = (m_min_bound + m_max_bound) * HALF;
}