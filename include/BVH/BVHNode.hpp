/**
 * @file BVHNode.hpp
 * @brief Header file for the BVHNode class.
 */
#ifndef BVH_BVHNODE_HPP
#define BVH_BVHNODE_HPP

#include <memory>

#include "Core/Math/Vec3.hpp"

/**
 * @class BVHNode
 * @brief Class representing a node in a Bounding Volume Hierarchy (BVH).
 *
 * This class encapsulates the properties of a BVH node, including its bounding box,
 * left and right children, and an index for leaf nodes.
 */
class BVHNode {
private:
  std::shared_ptr<BVHNode> m_left_child  = nullptr;
  std::shared_ptr<BVHNode> m_right_child = nullptr;

  int m_leaf_index = -1;

  lin::Vec3 m_min_bound = lin::Vec3(0.0);
  lin::Vec3 m_max_bound = lin::Vec3(0.0);
  lin::Vec3 m_center    = lin::Vec3(0.0);

public:
  BVHNode() = default; ///< Default constructor.

  BVHNode(const BVHNode&)            = delete;
  BVHNode& operator=(const BVHNode&) = delete;
  BVHNode(BVHNode&&)                 = delete;
  BVHNode& operator=(BVHNode&&)      = delete;

  /**
   * @brief Constructs a BVHNode with specified minimum and maximum bounds.
   *
   * @param min_bound The minimum bound of the bounding box.
   * @param max_bound The maximum bound of the bounding box.
   */
  BVHNode(const lin::Vec3& min_bound, const lin::Vec3& max_bound);

  /**
   * @brief Constructs a BVHNode with specified minimum and maximum bounds and a leaf index.
   *
   * @param min_bound The minimum bound of the bounding box.
   * @param max_bound The maximum bound of the bounding box.
   * @param leaf_index The index of the leaf node, or -1 if this is not a leaf node.
   */
  BVHNode(const lin::Vec3& min_bound, const lin::Vec3& max_bound, int leaf_index);

  /**
   * @brief Gets the minimum bound of the bounding box.
   *
   * @return The minimum bound as an lin::Vec3.
   */
  const lin::Vec3& getMinBound() const { return m_min_bound; }

  /**
   * @brief Gets the maximum bound of the bounding box.
   *
   * @return The maximum bound as an lin::Vec3.
   */
  const lin::Vec3& getMaxBound() const { return m_max_bound; }

  /**
   * @brief Gets the center of the bounding box.
   *
   * @return The center as an lin::Vec3.
   */
  const lin::Vec3& getCenter() const { return m_center; }

  /**
   * @brief Gets the leaf index of the BVH node.
   *
   * @return The leaf index, or -1 if this is not a leaf node.
   */
  int getLeafIndex() const { return m_leaf_index; }

  /**
   * @brief Gets the left child of the BVH node.
   *
   * @return A shared pointer to the left child BVHNode.
   */
  std::shared_ptr<BVHNode> getLeftChild() const { return m_left_child; }

  /**
   * @brief Gets the right child of the BVH node.
   *
   * @return A shared pointer to the right child BVHNode.
   */
  std::shared_ptr<BVHNode> getRightChild() const { return m_right_child; }

  /**
   * @brief Gets a reference to the left child of the BVH node.
   *
   * @return A reference to a shared pointer to the left child BVHNode.
   */
  std::shared_ptr<BVHNode>& getLeftChild() { return m_left_child; }

  /**
   * @brief Gets a reference to the right child of the BVH node.
   *
   * @return A reference to a shared pointer to the right child BVHNode.
   */
  std::shared_ptr<BVHNode>& getRightChild() { return m_right_child; }

  virtual ~BVHNode() = default; ///< Default destructor.
};

#endif // BVH_BVHNODE_HPP