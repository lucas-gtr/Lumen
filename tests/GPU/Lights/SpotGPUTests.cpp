#include <gtest/gtest.h>
#include "GPU/Lights/SpotGPU.hpp"

TEST(SpotGPUTest, Initialization) {
    SpotLight light;
    light.setPosition({1.0f, 2.0f, 3.0f});
    light.setDirection({0.0f, -1.0f, 0.0f});
    light.setOuterAngle(45.0f);
    light.setInnerAngle(30.0f);
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(1.0f);

    SpotLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getPosition(), linalg::Vec3f(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(gpu_light.getDirection(), linalg::Vec3f(0.0f, -1.0f, 0.0f).normalized());
    EXPECT_FLOAT_EQ(gpu_light.getCosInnerCutoff(), std::cos(30.0f * DEG_TO_RAD));
    EXPECT_FLOAT_EQ(gpu_light.getCosOuterCutoff(), std::cos(45.0f * DEG_TO_RAD));
    EXPECT_EQ(gpu_light.getColor(), linalg::Vec3f(1.0f, 1.0f, 1.0f));
}

TEST(SpotGPUTest, Position) {
    SpotLight light;
    light.setPosition({0.0f, 0.0f, -1.0f});
    SpotLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getPosition(), linalg::Vec3f(0.0f, 0.0f, -1.0f));
}

TEST(SpotGPUTest, Direction) {
    SpotLight light;
    light.setDirection({1.0f, -1.0f, 0.0f});
    SpotLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getDirection(), linalg::Vec3f(1.0f, -1.0f, 0.0f).normalized());
}

TEST(SpotGPUTest, Color) {
    SpotLight light;
    light.setColor({0.5f, 0.5f, 0.5f});
    SpotLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getColor(), linalg::Vec3f(0.5f, 0.5f, 0.5f));
}

TEST(SpotGPUTest, Intensity) {
    SpotLight light;
    light.setIntensity(2.0f);
    light.setColor({1.0f, 1.0f, 1.0f});
    SpotLightGPU gpu_light(&light);

    linalg::Vec3f expected_color = linalg::Vec3f(1.0f, 1.0f, 1.0f) * 2.0f;
    EXPECT_EQ(gpu_light.getColor(), expected_color);
}