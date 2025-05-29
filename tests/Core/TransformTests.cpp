#include "Core/Transform.hpp"

#include <gtest/gtest.h>


TEST(TransformTest, DefaultConstructorTest) {
    Transform t;
    EXPECT_EQ(t.getPosition(), lin::Vec3(0.0));
    EXPECT_EQ(t.getRotation(), lin::Vec3(0.0));
    EXPECT_EQ(t.getScale(), lin::Vec3(1.0));
}

TEST(TransformTest, ParameterizedConstructorTest) {
    lin::Vec3 position(1.0, 2.0, 3.0);
    lin::Vec3 rotation(30.0, 45.0, 60.0);
    lin::Vec3 scale(2.0, 2.0, 2.0);
    
    Transform t(position, rotation, scale);

    EXPECT_EQ(t.getPosition(), position);
    EXPECT_EQ(t.getRotation(), rotation * M_PI / 180.0);
    EXPECT_EQ(t.getScale(), scale);
}

TEST(TransformTest, SetPositionTest) {
    Transform t;
    lin::Vec3 newPosition(10.0, 20.0, 30.0);
    
    t.setPosition(newPosition);
    
    EXPECT_EQ(t.getPosition(), newPosition);
}

TEST(TransformTest, SetRotationTest) {
    Transform t;
    lin::Vec3 newRotation(45.0, 90.0, 135.0);
    
    t.setRotation(newRotation);
    
    EXPECT_EQ(t.getRotation(), newRotation * M_PI / 180.0);
}

TEST(TransformTest, SetScaleTest) {
    Transform t;
    lin::Vec3 newScale(2.0, 3.0, 4.0);
    
    t.setScale(newScale);
    
    EXPECT_EQ(t.getScale(), newScale);

    t.setScale(2.0);
    EXPECT_EQ(t.getScale(), lin::Vec3(2.0, 2.0, 2.0));
}

TEST(TransformTest, SetPositionXTest) {
    Transform t;
    t.setPositionX(5.0);
    EXPECT_EQ(t.getPosition(), lin::Vec3(5.0, 0.0, 0.0));
}

TEST(TransformTest, SetPositionYTest) {
    Transform t;
    t.setPositionY(10.0);
    EXPECT_EQ(t.getPosition(), lin::Vec3(0.0, 10.0, 0.0));
}

TEST(TransformTest, SetPositionZTest) {
    Transform t;
    t.setPositionZ(15.0);
    EXPECT_EQ(t.getPosition(), lin::Vec3(0.0, 0.0, 15.0));
}

TEST(TransformTest, SetRotationXTest) {
    Transform t;
    t.setRotationX(90.0);
    EXPECT_EQ(t.getRotation(), lin::Vec3(90.0 * M_PI / 180.0, 0.0, 0.0));
}

TEST(TransformTest, SetRotationYTest) {
    Transform t;
    t.setRotationY(180.0);
    EXPECT_EQ(t.getRotation(), lin::Vec3(0.0, 180.0 * M_PI / 180.0, 0.0));
}

TEST(TransformTest, SetRotationZTest) {
    Transform t;
    t.setRotationZ(270.0);
    EXPECT_EQ(t.getRotation(), lin::Vec3(0.0, 0.0, 270.0 * M_PI / 180.0));
}

TEST(TransformTest, SetScaleXTest) {
    Transform t;
    t.setScaleX(2.5);
    EXPECT_EQ(t.getScale(), lin::Vec3(2.5, 1.0, 1.0));
}

TEST(TransformTest, SetScaleYTest) {
    Transform t;
    t.setScaleY(3.5);
    EXPECT_EQ(t.getScale(), lin::Vec3(1.0, 3.5, 1.0));
}

TEST(TransformTest, SetScaleZTest) {
    Transform t;
    t.setScaleZ(4.5);
    EXPECT_EQ(t.getScale(), lin::Vec3(1.0, 1.0, 4.5));
}
TEST(TransformTest, TranslationMatrixTest) {
    lin::Vec3 position(5.0, 10.0, 15.0);
    Transform t;
    t.setPosition(position);

    lin::Mat4 expectedTranslationMatrix = lin::Mat4::Identity();
    expectedTranslationMatrix.m[0][3] = position.x;
    expectedTranslationMatrix.m[1][3] = position.y;
    expectedTranslationMatrix.m[2][3] = position.z;

    EXPECT_EQ(t.getTransformationMatrix(), expectedTranslationMatrix);
}

TEST(TransformTest, RotationMatrixTest) {
    lin::Vec3 rotation(45.0, 90.0, 30.0);
    Transform t;
    t.setRotation(rotation);

    lin::Mat3 rotationX, rotationY, rotationZ;
    double rx = rotation.x * M_PI / 180.0;
    double ry = rotation.y * M_PI / 180.0;
    double rz = rotation.z * M_PI / 180.0;

    rotationX = {{1, 0, 0},
                  {0, cos(rx), -sin(rx)},
                  {0, sin(rx), cos(rx)}};
    rotationY = {{cos(ry), 0, sin(ry)},
                  {0, 1, 0},
                  {-sin(ry), 0, cos(ry)}};
    rotationZ = {{cos(rz), -sin(rz), 0},
                  {sin(rz), cos(rz), 0},
                  {0, 0, 1}};

    lin::Mat3 expectedRotationMatrix = rotationZ * rotationY * rotationX;
    lin::Mat4 expectedTransformationMatrix(expectedRotationMatrix);

    EXPECT_TRUE(t.getTransformationMatrix().isApprox(expectedTransformationMatrix, 1e-9));
}

TEST(TransformTest, ScaleMatrixTest) {
    lin::Vec3 scale(2.0, 3.0, 4.0);
    Transform t;
    t.setScale(scale);

    lin::Mat4 expectedScaleMatrix = lin::Mat4::Identity();
    expectedScaleMatrix(0, 0) = scale.x;
    expectedScaleMatrix(1, 1) = scale.y;
    expectedScaleMatrix(2, 2) = scale.z;

    EXPECT_EQ(t.getTransformationMatrix(), expectedScaleMatrix);
}

TEST(TransformTest, CombinedTransformationMatrixTest) {
    lin::Vec3 position(5.0, 10.0, 15.0);
    lin::Vec3 rotation(45.0, 90.0, 30.0);
    lin::Vec3 scale(2.0, 3.0, 4.0);

    Transform t(position, rotation, scale);

    lin::Mat3 rotationX, rotationY, rotationZ;
    double rx = rotation.x * M_PI / 180.0;
    double ry = rotation.y * M_PI / 180.0;
    double rz = rotation.z * M_PI / 180.0;

    rotationX = {{1, 0, 0},
                  {0, cos(rx), -sin(rx)},
                  {0, sin(rx), cos(rx)}};
    rotationY = {{cos(ry), 0, sin(ry)},
                  {0, 1, 0},
                  {-sin(ry), 0, cos(ry)}};
    rotationZ = {{cos(rz), -sin(rz), 0},
                  {sin(rz), cos(rz), 0},
                  {0, 0, 1}};

    lin::Mat3 rotationMatrix = rotationZ * rotationY * rotationX;
    lin::Mat4 expectedTransformationMatrix(rotationMatrix);

    expectedTransformationMatrix.m[0][3] = position.x;
    expectedTransformationMatrix.m[1][3] = position.y;
    expectedTransformationMatrix.m[2][3] = position.z;


    expectedTransformationMatrix(0, 0) *= scale.x;
    expectedTransformationMatrix(0, 1) *= scale.x;
    expectedTransformationMatrix(0, 2) *= scale.x;
    expectedTransformationMatrix(1, 0) *= scale.y;
    expectedTransformationMatrix(1, 1) *= scale.y;
    expectedTransformationMatrix(1, 2) *= scale.y;
    expectedTransformationMatrix(2, 0) *= scale.z;
    expectedTransformationMatrix(2, 1) *= scale.z;
    expectedTransformationMatrix(2, 2) *= scale.z;

    EXPECT_TRUE(t.getTransformationMatrix().isApprox(expectedTransformationMatrix, 1e-9));
    EXPECT_TRUE(t.getInverseMatrix().isApprox(expectedTransformationMatrix.inverse(), 1e-9));
}

