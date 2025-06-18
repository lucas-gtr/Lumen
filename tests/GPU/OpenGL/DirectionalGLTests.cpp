#include <gtest/gtest.h>
#include "GPU/OpenGL/Lights/DirectionalGL.hpp"

TEST(DirectionalGLTest, Initialization) {
    DirectionalLight light;
    light.setDirection({1.0f, -1.0f, 0.0f});
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(1.0f);

    DirectionalLightGL gpu_light(&light);

    EXPECT_EQ(gpu_light.getDirection(), lin::Vec3f(1.0f, -1.0f, 0.0f).normalized());
    EXPECT_EQ(gpu_light.getColor(), lin::Vec3f(1.0f, 1.0f, 1.0f));
}

TEST(DirectionalGLTest, LightSpaceMatrix) {
    DirectionalLight light;
    light.setDirection({0.0f, 0.0f, -1.0f});
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(1.0f);

    DirectionalLightGL gpu_light(&light);
    gpu_light.updateLightSpaceMatrix();

    const float* matrixData = gpu_light.getLightSpaceMatrix();
    for (int i = 0; i < 16; ++i) {
        EXPECT_TRUE(std::isfinite(matrixData[i]));
    }
}
