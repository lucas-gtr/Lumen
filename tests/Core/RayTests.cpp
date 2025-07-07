#include "Core/Ray.hpp"

#include <gtest/gtest.h>

TEST(RayTest, ConstructorTest) {
    linalg::Vec3d origin(0.0, 0.0, 0.0);
    linalg::Vec3d point(1.0, 0.0, 0.0);
    
    Ray ray = Ray::FromPoint(origin, point);

    EXPECT_EQ(ray.origin, origin);

    EXPECT_EQ(ray.direction, linalg::Vec3d(1.0, 0.0, 0.0));
}

TEST(RayTest, NormalizedDirectionTest) {
    linalg::Vec3d origin(0.0, 0.0, 0.0);
    linalg::Vec3d point(1.0, 1.0, 0.0); 

    Ray ray = Ray::FromPoint(origin, point);

    EXPECT_NEAR(ray.direction.length(), 1.0, 1e-6);
}

TEST(RayTest, EqualDirectionTest) {
    linalg::Vec3d origin(0.0, 0.0, 0.0);
    linalg::Vec3d point(1.0, 0.0, 0.0);

    Ray ray1 = Ray::FromPoint(origin, point);
    Ray ray2 = Ray::FromPoint(origin, point);

    EXPECT_EQ(ray1.direction, ray2.direction);
}

TEST(RayTest, DirectionCalculationTest) {
    linalg::Vec3d origin(1.0, 2.0, 3.0);
    linalg::Vec3d point(4.0, 5.0, 6.0);
    
    Ray ray = Ray::FromPoint(origin, point);

    linalg::Vec3d expected_direction = (point - origin).normalized();
    EXPECT_EQ(ray.direction, expected_direction);
}

TEST(RayTest, FromDirectionTest) {
    linalg::Vec3d origin(0.0, 0.0, 0.0);
    linalg::Vec3d direction(1.0, 2.0, 3.0);

    Ray ray = Ray::FromDirection(origin, direction);

    EXPECT_EQ(ray.origin, origin);
    EXPECT_EQ(ray.direction, direction.normalized());
}

