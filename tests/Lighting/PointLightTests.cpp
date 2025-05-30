#include "Lighting/PointLight.hpp"

#include <gtest/gtest.h>

class PointLightTest : public ::testing::Test {
protected:
    PointLight light;

    void SetUp() override {
        light.setPosition(lin::Vec3(1.0, 2.0, 3.0));
    }
};

TEST_F(PointLightTest, ConstructorSetsTypeCorrectly) {
    EXPECT_EQ(light.getType(), LightType::Point);
}

TEST_F(PointLightTest, GetDirectionFromPointReturnsNormalizedVector) {
    lin::Vec3 point(4.0, 6.0, 8.0);
    lin::Vec3 direction = light.getDirectionFromPoint(point);

    lin::Vec3 expectedDirection = (light.getPosition() - point).normalized();
    EXPECT_NEAR(direction.x, expectedDirection.x, 1e-9);
    EXPECT_NEAR(direction.y, expectedDirection.y, 1e-9);
    EXPECT_NEAR(direction.z, expectedDirection.z, 1e-9);
}

TEST_F(PointLightTest, GetLightFactorConsidersDistanceAndNormalInclination) {
    lin::Vec3 pointNear(1.5, 2.5, 3.5);
    lin::Vec3 pointFar(10.0, 10.0, 10.0);

    lin::Vec3 normalFacing = (light.getPosition() - pointNear).normalized();
    
    lin::Vec3 normalSlightlyInclined = (light.getPosition() - pointNear).normalized();
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

