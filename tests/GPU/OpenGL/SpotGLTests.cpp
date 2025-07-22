#include <gtest/gtest.h>
#include "GPU/OpenGL/Lights/SpotGL.hpp"

TEST(SpotGLTest, Initialization) {
    SpotLight light;
    light.setPosition({1.0f, 2.0f, 3.0f});
    light.setDirection({0.0f, -1.0f, 0.0f});
    light.setOuterAngle(45.0f);
    light.setInnerAngle(30.0f);
    light.setColor({1.0f, 1.0f, 1.0f});
    light.setIntensity(1.0f);

    SpotLightGL gpu_light(&light);

    EXPECT_EQ(gpu_light.getPosition(), linalg::Vec3f(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(gpu_light.getDirection(), linalg::Vec3f(0.0f, -1.0f, 0.0f).normalized());
    EXPECT_FLOAT_EQ(gpu_light.getCosInnerCutoff(), std::cos(30.0f * DEG_TO_RAD));
    EXPECT_FLOAT_EQ(gpu_light.getCosOuterCutoff(), std::cos(45.0f * DEG_TO_RAD));
    EXPECT_EQ(gpu_light.getColor(), linalg::Vec3f(1.0f, 1.0f, 1.0f));
}

TEST(SpotLightGLTest, UpdateLightSpaceMatrixUpdatesFarPlaneAndMatrix) {
  SpotLight light;
  light.setPosition({1.0f, 2.0f, 3.0f});
  light.setDirection({0.0f, -1.0f, 0.0f});
  light.setIntensity(9.0f);

  SpotLightGL lightGL(&light);

  light.setIntensity(9.0f);
  lightGL.updateLightSpaceMatrix();

  float expectedFarPlane = std::sqrt(9.0f / MIN_LIGHT_INTENSITY_FAR_PLANE);
  EXPECT_NEAR(lightGL.getFarPlane(), expectedFarPlane, 1e-5f);

  const float* matrixData = lightGL.getLightSpaceMatrix();
  for (int i = 0; i < 16; ++i) {
    EXPECT_TRUE(std::isfinite(matrixData[i]));
  }
}

TEST(SpotLightGLTest, ObserverCallbacksUpdateDirectionAndMatrix) {
  SpotLight light;
  light.setPosition({1.0f, 2.0f, 3.0f});
  light.setDirection({0.0f, -1.0f, 0.0f});

  SpotLightGL lightGL(&light);

  EXPECT_EQ(lightGL.getDirection(), linalg::Vec3f(0.0f, -1.0f, 0.0f).normalized());

  light.setDirection({1.0f, 0.0f, 0.0f});
  EXPECT_EQ(lightGL.getDirection(), linalg::Vec3f(1.0f, 0.0f, 0.0f).normalized());

  light.setPosition({2.0f, 3.0f, 4.0f});
  EXPECT_EQ(lightGL.getPosition(), linalg::Vec3f(2.0f, 3.0f, 4.0f));
}