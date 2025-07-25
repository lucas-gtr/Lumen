#include <gtest/gtest.h>
#include "BVH/BVHNode.hpp"

TEST(BVHNodeTest, DefaultConstructorInitializesMembers) {
    BVHNode node;

    EXPECT_EQ(node.getLeafIndex(), -1);
    EXPECT_EQ(node.getLeftChild(), nullptr);
    EXPECT_EQ(node.getRightChild(), nullptr);
    EXPECT_TRUE(node.getMinBound().isApprox(linalg::Vec3d(0.0), 0.001));
    EXPECT_TRUE(node.getMaxBound().isApprox(linalg::Vec3d(0.0), 0.001));
    EXPECT_TRUE(node.getCenter().isApprox(linalg::Vec3d(0.0), 0.001));
}

TEST(BVHNodeTest, ConstructorWithBoundsComputesCenterCorrectly) {
    linalg::Vec3d min_bound(1.0, 1.0, 1.0);
    linalg::Vec3d max_bound(2.0, 2.0, 2.0);

    BVHNode node(min_bound, max_bound);

    EXPECT_TRUE(node.getMinBound().isApprox(min_bound, 0.001));
    EXPECT_TRUE(node.getMaxBound().isApprox(max_bound, 0.001));
    EXPECT_TRUE(node.getCenter().isApprox(linalg::Vec3d(1.5, 1.5, 1.5), 0.001));
    EXPECT_EQ(node.getLeafIndex(), -1);
}

TEST(BVHNodeTest, ConstructorWithBoundsAndLeafIndexSetsAllMembers) {
    linalg::Vec3d min_bound(-1.0, -1.0, -1.0);
    linalg::Vec3d max_bound(1.0, 1.0, 1.0);
    int leaf_index = 42;

    BVHNode node(min_bound, max_bound, leaf_index);

    EXPECT_TRUE(node.getMinBound().isApprox(min_bound, 0.001));
    EXPECT_TRUE(node.getMaxBound().isApprox(max_bound, 0.001));
    EXPECT_TRUE(node.getCenter().isApprox(linalg::Vec3d(0.0, 0.0, 0.0), 0.001));
    EXPECT_EQ(node.getLeafIndex(), leaf_index);
}

TEST(BVHNodeTest, ChildPointersCanBeAssignedAndRetrieved) {
    auto parent = std::make_shared<BVHNode>(
        linalg::Vec3d(0, 0, 0), linalg::Vec3d(1, 1, 1));
    auto left_child = std::make_shared<BVHNode>(
        linalg::Vec3d(0, 0, 0), linalg::Vec3d(0.5, 0.5, 0.5), 1);
    auto right_child = std::make_shared<BVHNode>(
        linalg::Vec3d(0.5, 0.5, 0.5), linalg::Vec3d(1, 1, 1), 2);

    parent->getLeftChild() = left_child;
    parent->getRightChild() = right_child;

    EXPECT_EQ(parent->getLeftChild(), left_child);
    EXPECT_EQ(parent->getRightChild(), right_child);
}
