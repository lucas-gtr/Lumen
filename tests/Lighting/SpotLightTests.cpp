#include "Lighting/SpotLight.hpp"
#include <gtest/gtest.h>
#include <Eigen/Dense>

class SpotLightTest : public ::testing::Test {
protected:
    SpotLight light;

    void SetUp() override {
        light.setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));
        light.setDirection(Eigen::Vector3d(0.0, 0.0, -1.0)); 
        light.setOuterAngle(30.0); 
        light.setInnerAngle(15.0); 
    }
};

TEST_F(SpotLightTest, ConstructorSetsTypeCorrectly) {
    EXPECT_EQ(light.getType(), LightType::Spot);
}

TEST_F(SpotLightTest, GetDirectionFromPointReturnsNormalizedVector) {
    Eigen::Vector3d point(0.0, 0.0, -5.0);
    Eigen::Vector3d direction = light.getDirectionFromPoint(point);

    Eigen::Vector3d expectedDirection = (light.getPosition() - point).normalized();
    EXPECT_NEAR(direction.x(), expectedDirection.x(), 1e-9);
    EXPECT_NEAR(direction.y(), expectedDirection.y(), 1e-9);
    EXPECT_NEAR(direction.z(), expectedDirection.z(), 1e-9);
}

TEST_F(SpotLightTest, LightFactorIsMaxInsideInnerCone) {
    Eigen::Vector3d point(0.0, 0.0, -1.0);
    Eigen::Vector3d normal(0.0, 0.0, 1.0);

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

  Eigen::Vector3d point(radius, 0.0, -1.0);
  Eigen::Vector3d normal(0.0, 0.0, 1.0);

  ColorRGB factor = light.getLightFactor(point, normal);

  double distance = (point - light.getPosition()).norm();
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
    Eigen::Vector3d point(offsetX * 5.0, 0.0, offsetZ * 5.0);
    Eigen::Vector3d normal(0.0, 0.0, 1.0);

    ColorRGB factor = light.getLightFactor(point, normal);

    EXPECT_NEAR(factor.r, 0.0, 1e-6);
    EXPECT_NEAR(factor.g, 0.0, 1e-6);
    EXPECT_NEAR(factor.b, 0.0, 1e-6);
}

TEST_F(SpotLightTest, LightFactorDependsOnNormalDirection) {
    Eigen::Vector3d point(0.0, 0.0, -5.0);

    Eigen::Vector3d normalFacingLight(0.0, 0.0, 1.0);
    ColorRGB factorFacing = light.getLightFactor(point, normalFacingLight);

    Eigen::Vector3d normalOppositeToLight(0.0, 0.0, -1.0);
    ColorRGB factorOpposite = light.getLightFactor(point, normalOppositeToLight);

    EXPECT_GT(factorFacing.r, factorOpposite.r);
    EXPECT_GT(factorFacing.g, factorOpposite.g);
    EXPECT_GT(factorFacing.b, factorOpposite.b);
}

