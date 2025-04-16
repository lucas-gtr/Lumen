#include "Core/Ray.hpp"

#include <gtest/gtest.h>
#include <Eigen/Core>

TEST(RayTest, DefaultConstructorTest) {
    Ray ray;

    EXPECT_EQ(ray.origin, Eigen::Vector3d::Zero());
    EXPECT_EQ(ray.direction, Eigen::Vector3d(0.0, 0.0, -1.0));
}

TEST(RayTest, ConstructorTest) {
    Eigen::Vector3d origin(0.0, 0.0, 0.0);
    Eigen::Vector3d point(1.0, 0.0, 0.0);
    
    Ray ray(origin, point);

    EXPECT_EQ(ray.origin, origin);

    EXPECT_EQ(ray.direction, Eigen::Vector3d(1.0, 0.0, 0.0));
}

TEST(RayTest, NormalizedDirectionTest) {
    Eigen::Vector3d origin(0.0, 0.0, 0.0);
    Eigen::Vector3d point(1.0, 1.0, 0.0); 

    Ray ray(origin, point);

    EXPECT_NEAR(ray.direction.norm(), 1.0, 1e-6);
}

TEST(RayTest, EqualDirectionTest) {
    Eigen::Vector3d origin(0.0, 0.0, 0.0);
    Eigen::Vector3d point(1.0, 0.0, 0.0);

    Ray ray1(origin, point);
    Ray ray2(origin, point);

    EXPECT_EQ(ray1.direction, ray2.direction);
}

TEST(RayTest, DirectionCalculationTest) {
    Eigen::Vector3d origin(1.0, 2.0, 3.0);
    Eigen::Vector3d point(4.0, 5.0, 6.0);
    
    Ray ray(origin, point);

    Eigen::Vector3d expected_direction = (point - origin).normalized();
    EXPECT_EQ(ray.direction, expected_direction);
}

