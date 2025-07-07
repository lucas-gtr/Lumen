#include "Lighting/PointLight.hpp"

#include <gtest/gtest.h>

class PointLightTest : public ::testing::Test {
protected:
    PointLight light;

    void SetUp() override {
        light.setPosition(linalg::Vec3d(1.0, 2.0, 3.0));
    }
};

TEST_F(PointLightTest, ConstructorSetsTypeCorrectly) {
    EXPECT_EQ(light.getType(), LightType::POINT);
}

TEST_F(PointLightTest, GetDirectionFromPointReturnsNormalizedVector) {
    linalg::Vec3d point(4.0, 6.0, 8.0);
    linalg::Vec3d direction = light.getDirectionFromPoint(point);

    linalg::Vec3d expectedDirection = (light.getPosition() - point).normalized();
    EXPECT_NEAR(direction.x, expectedDirection.x, 1e-9);
    EXPECT_NEAR(direction.y, expectedDirection.y, 1e-9);
    EXPECT_NEAR(direction.z, expectedDirection.z, 1e-9);
}

TEST_F(PointLightTest, GetLightFactorConsidersDistanceAndNormalInclination) {
    linalg::Vec3d pointNear(1.5, 2.5, 3.5);
    linalg::Vec3d pointFar(10.0, 10.0, 10.0);

    linalg::Vec3d normalFacing = (light.getPosition() - pointNear).normalized();
    
    linalg::Vec3d normalSlightlyInclined = (light.getPosition() - pointNear).normalized();
    normalSlightlyInclined.x += 0.1; 
    normalSlightlyInclined.normalize();

    ColorRGB factorNearFacing = light.getLightFactor(pointNear, normalFacing);
    ColorRGB factorNearInclined = light.getLightFactor(pointNear, normalSlightlyInclined);
    ColorRGB factorFarFacing = light.getLightFactor(pointFar, normalFacing);

    EXPECT_GT(factorNearFacing.r, factorNearInclined.r);
    EXPECT_GT(factorNearFacing.g, factorNearInclined.g);
    EXPECT_GT(factorNearFacing.b, factorNearInclined.b);
    EXPECT_LT(factorFarFacing.r, factorNearFacing.r);
    EXPECT_LT(factorFarFacing.g, factorNearFacing.g);   
    EXPECT_LT(factorFarFacing.b, factorNearFacing.b);
}

