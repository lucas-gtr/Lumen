#include "Lighting/DirectionalLight.hpp"

#include <gtest/gtest.h>

TEST(DirectionalLightTest, DefaultConstructor) {
    DirectionalLight light;
    EXPECT_EQ(light.getDirection(), lin::Vec3d(0.0, 0.0, -1.0));
}

TEST(DirectionalLightTest, SetDirection) {
    DirectionalLight light;
    lin::Vec3d direction(1.0, 0.0, 0.0);
    light.setDirection(direction);
    EXPECT_EQ(light.getDirection(), direction.normalized());
}

TEST(DirectionalLightTest, GetDirection) {
    DirectionalLight light;
    lin::Vec3d direction(0.0, 1.0, 0.0);
    light.setDirection(direction);
    EXPECT_EQ(light.getDirection(), direction.normalized());
}

TEST(DirectionalLightTest, GetDirectionFromPointDefault) {
    DirectionalLight light;
    lin::Vec3d point(1.0, 2.0, 3.0);
    lin::Vec3d direction = light.getDirectionFromPoint(point);
    EXPECT_EQ(direction, lin::Vec3d(0.0, 0.0, 1.0));
}

TEST(DirectionalLightTest, GetDirectionFromPoint) {
    DirectionalLight light;
    lin::Vec3d point(1.0, 2.0, 3.0);
    lin::Vec3d direction(2.0, 0.0, 0.0);
    light.setDirection(direction);
    lin::Vec3d result = light.getDirectionFromPoint(point);
    EXPECT_EQ(result, -direction.normalized());
}

TEST(DirectionalLightTest, GetLightFactor) {
    DirectionalLight light;
    lin::Vec3d point(1.0, 2.0, 3.0);
    lin::Vec3d normal(0.0, 0.0, 1.0);
    light.setColor({0.2, 0.3, 0.4});
    light.setIntensity(2.0);
    ColorRGB factor = light.getLightFactor(point, normal);
    EXPECT_EQ(factor, ColorRGB(0.4, 0.6, 0.8));
}
