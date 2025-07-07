#include "Lighting/DirectionalLight.hpp"

#include <gtest/gtest.h>

TEST(DirectionalLightTest, DefaultConstructor) {
    DirectionalLight light;
    EXPECT_EQ(light.getDirection(), linalg::Vec3d(0.0, 0.0, -1.0));
    EXPECT_EQ(light.getType(), LightType::DIRECTIONAL);
}

TEST(DirectionalLightTest, SetDirection) {
    DirectionalLight light;
    linalg::Vec3d direction(1.0, 0.0, 0.0);
    light.setDirection(direction);
    EXPECT_EQ(light.getDirection(), direction.normalized());
}

TEST(DirectionalLightTest, GetDirection) {
    DirectionalLight light;
    linalg::Vec3d direction(0.0, 1.0, 0.0);
    light.setDirection(direction);
    EXPECT_EQ(light.getDirection(), direction.normalized());
}

TEST(DirectionalLightTest, GetDirectionFromPointDefault) {
    DirectionalLight light;
    linalg::Vec3d point(1.0, 2.0, 3.0);
    linalg::Vec3d direction = light.getDirectionFromPoint(point);
    EXPECT_EQ(direction, linalg::Vec3d(0.0, 0.0, 1.0));
}

TEST(DirectionalLightTest, GetDirectionFromPoint) {
    DirectionalLight light;
    linalg::Vec3d point(1.0, 2.0, 3.0);
    linalg::Vec3d direction(2.0, 0.0, 0.0);
    light.setDirection(direction);
    linalg::Vec3d result = light.getDirectionFromPoint(point);
    EXPECT_EQ(result, -direction.normalized());
}

TEST(DirectionalLightTest, GetLightFactor) {
    DirectionalLight light;
    linalg::Vec3d point(1.0, 2.0, 3.0);
    linalg::Vec3d normal(0.0, 0.0, 1.0);
    light.setColor({0.2, 0.3, 0.4});
    light.setIntensity(2.0);
    ColorRGB factor = light.getLightFactor(point, normal);
    EXPECT_EQ(factor, ColorRGB(0.4, 0.6, 0.8));
}
