#include "Lighting/SpotLight.hpp"
#include <gtest/gtest.h>

class SpotLightTest : public ::testing::Test {
protected:
    SpotLight light;

    void SetUp() override {
        light.setPosition(linalg::Vec3d(0.0, 0.0, 0.0));
        light.setDirection(linalg::Vec3d(0.0, 0.0, -1.0)); 
        light.setOuterAngle(30.0); 
        light.setInnerAngle(15.0); 
    }
};

TEST_F(SpotLightTest, ConstructorSetsTypeCorrectly) {
    EXPECT_EQ(light.getType(), LightType::SPOT);
}

TEST_F(SpotLightTest, GetDirection) {
    light.setDirection(linalg::Vec3d(1.0, 0.0, 0.0));
    linalg::Vec3d direction = light.getDirection();
    EXPECT_DOUBLE_EQ(direction.x, 1.0);
    EXPECT_DOUBLE_EQ(direction.y, 0.0);
    EXPECT_DOUBLE_EQ(direction.z, 0.0);
}

TEST_F(SpotLightTest, GetInnerAngleRad) {
    light.setInnerAngle(15.0);
    EXPECT_DOUBLE_EQ(light.getInnerAngleRad(), 15.0 * M_PI / 180.0); 
}

TEST_F(SpotLightTest, GetOuterAngleRad) {
    light.setOuterAngle(30.0);
    EXPECT_DOUBLE_EQ(light.getOuterAngleRad(), 30.0 * M_PI / 180.0); 
}

TEST_F(SpotLightTest, GetInnerAngleDeg) {
    light.setInnerAngle(15.0);
    EXPECT_DOUBLE_EQ(light.getInnerAngleDeg(), 15.0);
}

TEST_F(SpotLightTest, GetOuterAngleDeg) {
    light.setOuterAngle(30.0);
    EXPECT_DOUBLE_EQ(light.getOuterAngleDeg(), 30.0);
}

TEST_F(SpotLightTest, GetDirectionFromPointReturnsNormalizedVector) {
    linalg::Vec3d point(0.0, 0.0, -5.0);
    linalg::Vec3d direction = light.getDirectionFromPoint(point);

    linalg::Vec3d expectedDirection = (light.getPosition() - point).normalized();
    EXPECT_NEAR(direction.x, expectedDirection.x, 1e-9);
    EXPECT_NEAR(direction.y, expectedDirection.y, 1e-9);
    EXPECT_NEAR(direction.z, expectedDirection.z, 1e-9);
}

TEST_F(SpotLightTest, LightFactorIsMaxInsideInnerCone) {
    linalg::Vec3d point(0.0, 0.0, -1.0);
    linalg::Vec3d normal(0.0, 0.0, 1.0);

    ColorRGB factor = light.getLightFactor(point, normal);

    EXPECT_NEAR(factor.r, 1.0, 1e-6);
    EXPECT_NEAR(factor.g, 1.0, 1e-6);
    EXPECT_NEAR(factor.b, 1.0, 1e-6);
}

TEST_F(SpotLightTest, LightFactorIsHalfwayBasedOnCosine) {
  double innerCos = std::cos(15.0 * M_PI / 180.0);
  double outerCos = std::cos(30.0 * M_PI / 180.0);

  double halfwayCos = (innerCos + outerCos) / 2.0; 
    
  double angle = std::acos(halfwayCos);

  float radius = std::tan(angle);

  linalg::Vec3d point(radius, 0.0, -1.0);
  linalg::Vec3d normal(0.0, 0.0, 1.0);

  ColorRGB factor = light.getLightFactor(point, normal);

  double distance = (point - light.getPosition()).length();
  double attenuation = 1.0 / (distance * distance);

  double expected = attenuation * 0.5;
  EXPECT_NEAR(factor.r, expected, 0.05);
  EXPECT_NEAR(factor.g, expected, 0.05);
  EXPECT_NEAR(factor.b, expected, 0.05);
}

TEST_F(SpotLightTest, LightFactorIsZeroOutsideOuterCone) {
    double angle = 45.0; 
    double radians = angle * M_PI / 180.0;
    double offsetX = sin(radians);
    double offsetZ = -cos(radians);
    linalg::Vec3d point(offsetX * 5.0, 0.0, offsetZ * 5.0);
    linalg::Vec3d normal(0.0, 0.0, 1.0);

    ColorRGB factor = light.getLightFactor(point, normal);

    EXPECT_NEAR(factor.r, 0.0, 1e-6);
    EXPECT_NEAR(factor.g, 0.0, 1e-6);
    EXPECT_NEAR(factor.b, 0.0, 1e-6);
}

TEST_F(SpotLightTest, LightFactorDependsOnNormalDirection) {
    linalg::Vec3d point(0.0, 0.0, -5.0);

    linalg::Vec3d normalFacingLight(0.0, 0.0, 1.0);
    ColorRGB factorFacing = light.getLightFactor(point, normalFacingLight);

    linalg::Vec3d normalOppositeToLight(0.0, 0.0, -1.0);
    ColorRGB factorOpposite = light.getLightFactor(point, normalOppositeToLight);

    EXPECT_GT(factorFacing.r, factorOpposite.r);
    EXPECT_GT(factorFacing.g, factorOpposite.g);
    EXPECT_GT(factorFacing.b, factorOpposite.b);
}

