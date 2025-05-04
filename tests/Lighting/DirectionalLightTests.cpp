#include "Lighting/DirectionalLight.hpp"

#include <gtest/gtest.h>

TEST(DirectionalLightTest, DefaultConstructor) {
    DirectionalLight light;
    EXPECT_EQ(light.getDirection(), Eigen::Vector3d(0.0, 0.0, -1.0));
}

TEST(DirectionalLightTest, SetDirection) {
    DirectionalLight light;
    Eigen::Vector3d direction(1.0, 0.0, 0.0);
    light.setDirection(direction);
    EXPECT_EQ(light.getDirection(), direction.normalized());
}

TEST(DirectionalLightTest, GetDirection) {
    DirectionalLight light;
    Eigen::Vector3d direction(0.0, 1.0, 0.0);
    light.setDirection(direction);
    EXPECT_EQ(light.getDirection(), direction.normalized());
}

TEST(DirectionalLightTest, GetDirectionFromPointDefault) {
    DirectionalLight light;
    Eigen::Vector3d point(1.0, 2.0, 3.0);
    Eigen::Vector3d direction = light.getDirectionFromPoint(point);
    EXPECT_EQ(direction, Eigen::Vector3d(0.0, 0.0, 1.0));
}

TEST(DirectionalLightTest, GetDirectionFromPoint) {
    DirectionalLight light;
    Eigen::Vector3d point(1.0, 2.0, 3.0);
    Eigen::Vector3d direction(2.0, 0.0, 0.0);
    light.setDirection(direction);
    Eigen::Vector3d result = light.getDirectionFromPoint(point);
    EXPECT_EQ(result, -direction.normalized());
}

TEST(DirectionalLightTest, GetLightFactor) {
    DirectionalLight light;
    Eigen::Vector3d point(1.0, 2.0, 3.0);
    Eigen::Vector3d normal(0.0, 0.0, 1.0);
    light.setColor(Eigen::Vector3d(1.0, 1.0, 1.0));
    light.setIntensity(1.0);
    Eigen::Vector3d factor = light.getLightFactor(point, normal);
    EXPECT_EQ(factor, Eigen::Vector3d(1.0, 1.0, 1.0));
}
