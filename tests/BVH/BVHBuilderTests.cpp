#include "BVH/BVHBuilder.hpp"

#include <gtest/gtest.h>

TEST(BVHBuilderTest, AxisSelection) {
    lin::Vec3 extentX = lin::Vec3(3, 2, 1);
    lin::Vec3 extentY = lin::Vec3(1, 3, 2);
    lin::Vec3 extentZ = lin::Vec3(1, 2, 3);

    EXPECT_EQ(BVH::getLargestAxis(extentX), 0);
    EXPECT_EQ(BVH::getLargestAxis(extentY), 1);
    EXPECT_EQ(BVH::getLargestAxis(extentZ), 2);
}

TEST(BVHBuilderTest, EndSuperiorToStart) {
    std::vector<std::shared_ptr<BVHNode>> objects;
    std::shared_ptr<BVHNode> obj1 = std::make_shared<BVHNode>(lin::Vec3(0, 0, 0), lin::Vec3(1, 1, 1));
    std::shared_ptr<BVHNode> obj2 = std::make_shared<BVHNode>(lin::Vec3(1, 1, 1), lin::Vec3(2, 2, 2));
    objects.push_back(obj1);
    objects.push_back(obj2);

    std::shared_ptr<BVHNode> root;
    BVH::constructNode(root, objects, 1, 0);

    EXPECT_EQ(root, nullptr);
}

TEST(BVHBuilderTest, OneObjectTest) {
    std::vector<std::shared_ptr<BVHNode>> objects;
    std::shared_ptr<BVHNode> obj = std::make_shared<BVHNode>(lin::Vec3(0, 0, 0), lin::Vec3(1, 1, 1));
    objects.push_back(obj);

    std::shared_ptr<BVHNode> root;
    BVH::constructNode(root, objects, 0, static_cast<int>(objects.size()));

    EXPECT_EQ(root, obj);
    EXPECT_TRUE(root->getMinBound().isApprox(lin::Vec3(0, 0, 0), 0.001));
    EXPECT_TRUE(root->getMaxBound().isApprox(lin::Vec3(1, 1, 1), 0.001));
}

TEST(BVHBuilderTest, ConstructNodeTest) {
    std::vector<std::shared_ptr<BVHNode>> objects;
    std::shared_ptr<BVHNode> obj1 = std::make_shared<BVHNode>(lin::Vec3(0, 0, 0), lin::Vec3(1, 1, 1));
    std::shared_ptr<BVHNode> obj2 = std::make_shared<BVHNode>(lin::Vec3(1, 1, 1), lin::Vec3(2, 2, 2));
    std::shared_ptr<BVHNode> obj3 = std::make_shared<BVHNode>(lin::Vec3(2, 2, 2), lin::Vec3(3, 3, 3));
    objects.push_back(obj1);
    objects.push_back(obj2);
    objects.push_back(obj3);

    std::shared_ptr<BVHNode> root;
    BVH::constructNode(root, objects, 0, static_cast<int>(objects.size()));

    EXPECT_NE(root, nullptr);
    EXPECT_TRUE(root->getMinBound().isApprox(lin::Vec3(0, 0, 0), 0.001));
    EXPECT_TRUE(root->getMaxBound().isApprox(lin::Vec3(3, 3, 3), 0.001));
    EXPECT_TRUE(root->getLeftChild()->getMinBound().isApprox(lin::Vec3(0, 0, 0), 0.001));
    EXPECT_TRUE(root->getLeftChild()->getMaxBound().isApprox(lin::Vec3(1, 1, 1), 0.001));
    EXPECT_TRUE(root->getRightChild()->getMinBound().isApprox(lin::Vec3(1, 1, 1), 0.001));
    EXPECT_TRUE(root->getRightChild()->getMaxBound().isApprox(lin::Vec3(3, 3, 3), 0.001));
    EXPECT_EQ(root->getLeftChild(), obj1);
    EXPECT_EQ(root->getRightChild()->getLeftChild(), obj2);
    EXPECT_EQ(root->getRightChild()->getRightChild(), obj3);
}

