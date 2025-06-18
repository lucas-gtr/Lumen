#include <gtest/gtest.h>
#include "GPU/OpenGL/Lights/PointGL.hpp"

TEST(PointGLTest, Initialization) {
    PointLight light;
    light.setPosition({1.0f, 2.0f, 3.0f});
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(2.0f);

    PointLightGL gpu_light(&light);

    EXPECT_EQ(gpu_light.getPosition(), lin::Vec3f(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(gpu_light.getColor(), lin::Vec3f(1.0f, 1.0f, 1.0f) * 2.0);
}

TEST(PointLightGLTest, UpdateLightSpaceMatrixUpdatesFarPlaneAndMatrix) {
    PointLight light;
    light.setPosition({1.0f, 2.0f, 3.0f});
    light.setIntensity(9.0f);

    PointLightGL lightGL(&light);
    lightGL.updateLightSpaceMatrices();

    float expectedFarPlane = std::sqrt(9.0f / MIN_LIGHT_INTENSITY_FAR_PLANE);
    EXPECT_NEAR(lightGL.getFarPlane(), expectedFarPlane, 1e-5f);

    for(int i = 0; i < 6; ++i) {
      const float* matrixData = lightGL.getLightSpaceMatrix(i);
      for (int j = 0; j < 16; ++j) {
          EXPECT_TRUE(std::isfinite(matrixData[j]));
      }
    }
}