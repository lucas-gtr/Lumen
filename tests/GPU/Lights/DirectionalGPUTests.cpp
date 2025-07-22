#include <gtest/gtest.h>
#include "GPU/Lights/DirectionalGPU.hpp"

TEST(DirectionalGPUTest, Initialization) {
    DirectionalLight light;
    light.setDirection({1.0f, -1.0f, 0.0f});
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(1.0f);

    DirectionalLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getDirection(), linalg::Vec3f(1.0f, -1.0f, 0.0f).normalized());
    EXPECT_EQ(gpu_light.getColor(), linalg::Vec3f(1.0f, 1.0f, 1.0f));
}

TEST(DirectionalGPUTest, Direction) {
    DirectionalLight light;
    light.setDirection({0.0f, 0.0f, -1.0f});
    DirectionalLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getDirection(), linalg::Vec3f(0.0f, 0.0f, -1.0f));
}

TEST(DirectionalGPUTest, Color) {
    DirectionalLight light;
    light.setColor({0.5f, 0.5f, 0.5f});
    DirectionalLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getColor(), linalg::Vec3f(0.5f, 0.5f, 0.5f));
}
TEST(DirectionalGPUTest, Intensity) {
    DirectionalLight light;
    light.setIntensity(2.0f);
    light.setColor({1.0f, 1.0f, 1.0f});
    DirectionalLightGPU gpu_light(&light);

    linalg::Vec3f expected_color = linalg::Vec3f(1.0f, 1.0f, 1.0f) * 2.0f;
    EXPECT_EQ(gpu_light.getColor(), expected_color);
}

TEST(DirectionalGPUTest, GetSource) {
    DirectionalLight light;
    DirectionalLightGPU gpu_light(&light);

    EXPECT_EQ(gpu_light.getSource(), &light);
}