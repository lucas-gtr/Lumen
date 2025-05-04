#include "Lighting/PointLight.hpp"

#include <gtest/gtest.h>
#include <Eigen/Dense>

class PointLightTest : public ::testing::Test {
protected:
    PointLight light;

    void SetUp() override {
        light.setPosition(Eigen::Vector3d(1.0, 2.0, 3.0));
    }
};

TEST_F(PointLightTest, ConstructorSetsTypeCorrectly) {
    EXPECT_EQ(light.getType(), LightType::Point);
}

TEST_F(PointLightTest, GetDirectionFromPointReturnsNormalizedVector) {
    Eigen::Vector3d point(4.0, 6.0, 8.0);
    Eigen::Vector3d direction = light.getDirectionFromPoint(point);

    Eigen::Vector3d expectedDirection = (light.getPosition() - point).normalized();
    EXPECT_NEAR(direction.x(), expectedDirection.x(), 1e-9);
    EXPECT_NEAR(direction.y(), expectedDirection.y(), 1e-9);
    EXPECT_NEAR(direction.z(), expectedDirection.z(), 1e-9);
}

TEST_F(PointLightTest, GetLightFactorConsidersDistanceAndNormalInclination) {
    Eigen::Vector3d pointNear(1.5, 2.5, 3.5);
    Eigen::Vector3d pointFar(10.0, 10.0, 10.0);

    Eigen::Vector3d normalFacing = (light.getPosition() - pointNear).normalized();
    
    Eigen::Vector3d normalSlightlyInclined = (light.getPosition() - pointNear).normalized();
    normalSlightlyInclined.x() += 0.1; 
    normalSlightlyInclined.normalize();

    Eigen::Vector3d factorNearFacing = light.getLightFactor(pointNear, normalFacing);
    Eigen::Vector3d factorNearInclined = light.getLightFactor(pointNear, normalSlightlyInclined);
    Eigen::Vector3d factorFarFacing = light.getLightFactor(pointFar, normalFacing);

    EXPECT_GT(factorNearFacing.norm(), factorNearInclined.norm());
    EXPECT_GT(factorNearFacing.norm(), factorFarFacing.norm());
}

