#include <gtest/gtest.h>
#include "GPU/Lights/PointGPU.hpp"

TEST(PointGPUTest, Initialization) {
    PointLight light;
    light.setPosition({1.0f, 2.0f, 3.0f});
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(1.0f);

    PointLightGPU gpu_light(light);

    EXPECT_EQ(gpu_light.getPosition(), lin::Vec3f(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(gpu_light.getColor(), lin::Vec3f(1.0f, 1.0f, 1.0f));
}

TEST(PointGPUTest, Position) {
    PointLight light;
    light.setPosition({0.0f, 0.0f, -1.0f});
    PointLightGPU gpu_light(light);

    EXPECT_EQ(gpu_light.getPosition(), lin::Vec3f(0.0f, 0.0f, -1.0f));
}

TEST(PointGPUTest, Color) {
    PointLight light;
    light.setColor({0.5f, 0.5f, 0.5f});
    PointLightGPU gpu_light(light);

    EXPECT_EQ(gpu_light.getColor(), lin::Vec3f(0.5f, 0.5f, 0.5f));
}

TEST(PointGPUTest, Intensity) {
    PointLight light;
    light.setIntensity(2.0f);
    light.setColor({1.0f, 1.0f, 1.0f});
    PointLightGPU gpu_light(light);

    lin::Vec3f expected_color = lin::Vec3f(1.0f, 1.0f, 1.0f) * 2.0f;
    EXPECT_EQ(gpu_light.getColor(), expected_color);
}