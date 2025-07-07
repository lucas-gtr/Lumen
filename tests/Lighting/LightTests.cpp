#include "Lighting/Light.hpp"

#include <gtest/gtest.h>

class MockLight : public Light {
  public:
      explicit MockLight(LightType type) : Light(type) {}
  
      linalg::Vec3d getDirectionFromPoint(const linalg::Vec3d& point) const override {
          return linalg::Vec3d(1.0, 0.0, 0.0);
      }
  
      ColorRGB getLightFactor(const linalg::Vec3d& point, const linalg::Vec3d& normal) const override {
          return ColorRGB(1.0, 0.9, 0.8);
      }
  };
  
  TEST(LightTest, ConstructorSetsType) {
      MockLight light(LightType::POINT);
      EXPECT_EQ(light.getType(), LightType::POINT);
  }
  
  TEST(LightTest, DefaultColorIsWhite) {
      MockLight light(LightType::DIRECTIONAL);
      ColorRGB expected_color(1.0, 1.0, 1.0);
      EXPECT_EQ(light.getColor(), expected_color);
  }
  
  TEST(LightTest, DefaultIntensityIsOne) {
      MockLight light(LightType::SPOT);
      EXPECT_DOUBLE_EQ(light.getIntensity(), 1.0);
  }
  
  TEST(LightTest, SetColorChangesColor) {
      MockLight light(LightType::POINT);
      ColorRGB new_color(0.5, 0.4, 0.3);
      light.setColor(new_color);
      EXPECT_EQ(light.getColor(), new_color);
  }
  
  TEST(LightTest, SetIntensityChangesIntensity) {
      MockLight light(LightType::DIRECTIONAL);
      double new_intensity = 2.5;
      light.setIntensity(new_intensity);
      EXPECT_DOUBLE_EQ(light.getIntensity(), new_intensity);
  }
  
  TEST(LightTest, GetDirectionFromPointReturnsExpectedVector) {
      MockLight light(LightType::POINT);
      linalg::Vec3d point(0.0, 0.0, 0.0);
      linalg::Vec3d expected_direction(1.0, 0.0, 0.0);
      EXPECT_EQ(light.getDirectionFromPoint(point), expected_direction);
  }
  
  TEST(LightTest, GetLightFactorReturnsExpectedVector) {
      MockLight light(LightType::POINT);
      linalg::Vec3d point(0.0, 0.0, 0.0);
      linalg::Vec3d normal(0.0, 1.0, 0.0);
      ColorRGB expected_light_factor(1.0, 0.9, 0.8);
      EXPECT_EQ(light.getLightFactor(point, normal), expected_light_factor);
  }
  