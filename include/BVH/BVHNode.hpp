#ifndef BVH_BVHNODE_HPP
#define BVH_BVHNODE_HPP

#include <Eigen/Core>
#include <memory>

class BVHNode {
private:
  std::shared_ptr<BVHNode> m_left_child  = nullptr;
  std::shared_ptr<BVHNode> m_right_child = nullptr;

  Eigen::Vector3d m_min_bound = Eigen::Vector3d::Zero();
  Eigen::Vector3d m_max_bound = Eigen::Vector3d::Zero();
  Eigen::Vector3d m_center = Eigen::Vector3d::Zero();

  int m_leaf_index = -1;

public:
  BVHNode() = default;

  BVHNode(const BVHNode&)            = delete;
  BVHNode& operator=(const BVHNode&) = delete;
  BVHNode(BVHNode&&)                 = delete;
  BVHNode& operator=(BVHNode&&)      = delete;

  BVHNode(const Eigen::Vector3d& min_bound, const Eigen::Vector3d& max_bound);
  BVHNode(const Eigen::Vector3d& min_bound, const Eigen::Vector3d& max_bound, int leaf_index);

  const Eigen::Vector3d& getMinBound() const { return m_min_bound; }
  const Eigen::Vector3d& getMaxBound() const { return m_max_bound; }
  const Eigen::Vector3d& getCenter() const { return m_center; }
  int                    getLeafIndex() const { return m_leaf_index; }

  std::shared_ptr<BVHNode> getLeftChild() const { return m_left_child; }
  std::shared_ptr<BVHNode> getRightChild() const { return m_right_child; }

  std::shared_ptr<BVHNode>& getLeftChild() { return m_left_child; }
  std::shared_ptr<BVHNode>& getRightChild() { return m_right_child; }

  virtual ~BVHNode() = default;
};

#endif // BVH_BVHNODE_HPP