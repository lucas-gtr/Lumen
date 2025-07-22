#include "SceneObjects/Camera.hpp"

#include <gtest/gtest.h>

TEST(CameraTest, TestDefaultConstructor) {
    Camera camera;
    EXPECT_DOUBLE_EQ(camera.getFovX(), 45.0);
    EXPECT_DOUBLE_EQ(camera.getFocalLength(), 0.05);
    EXPECT_DOUBLE_EQ(camera.getAperture(), 2.8);
    EXPECT_DOUBLE_EQ(camera.getShutterSpeed(), 1.0 / 60.0);
    EXPECT_DOUBLE_EQ(camera.getFocusDistance(), 6.0);
    EXPECT_DOUBLE_EQ(camera.getSensorWidth(), 2 * 0.05 * tan(45.0 * 0.5 * DEG_TO_RAD));
    EXPECT_DOUBLE_EQ(camera.getLensRadius(), 0.05 / 2.8 / 2.0);
    EXPECT_DOUBLE_EQ(camera.getNearPlane(), 0.1);
    EXPECT_DOUBLE_EQ(camera.getFarPlane(), 500.0);
}

TEST(CameraTest, TestFovMinMax) {
    Camera camera;
    camera.setHorizontalFov(29.0);
    EXPECT_DOUBLE_EQ(camera.getFovX(), 30.0);

    camera.setHorizontalFov(121.0);
    EXPECT_DOUBLE_EQ(camera.getFovX(), 120.0);

    camera.setHorizontalFov(60.0);
    EXPECT_DOUBLE_EQ(camera.getFovX(), 60.0);
}

TEST(CameraTest, TestSensorWidthMinMax) {
    Camera camera;
    camera.setHorizontalFov(30.0);
    double min_sensor_width = camera.getSensorWidth();
    camera.setSensorWidth(0.0);
    EXPECT_DOUBLE_EQ(camera.getSensorWidth(), min_sensor_width);

    camera.setHorizontalFov(120.0);
    double max_sensor_width = camera.getSensorWidth();
    camera.setSensorWidth((max_sensor_width * 1000.0) + 10.0);
    EXPECT_DOUBLE_EQ(camera.getSensorWidth(), max_sensor_width);

    camera.setSensorWidth(50.0);
    EXPECT_DOUBLE_EQ(camera.getSensorWidth(), 0.05);
}

TEST(CameraTest, TestFovSensorWidthInteraction) {
    Camera camera;
    camera.setHorizontalFov(60.0);
    double initial_fov = camera.getFovX();
    double initial_sensor_width = camera.getSensorWidth();

    camera.setHorizontalFov(90.0);
    EXPECT_NE(camera.getFovX(), initial_fov);
    EXPECT_NE(camera.getSensorWidth(), initial_sensor_width);

    camera.setSensorWidth(initial_sensor_width * 1000.0);
    EXPECT_DOUBLE_EQ(camera.getFovX(), initial_fov);

    camera.setSensorWidth(initial_sensor_width * 1000.0 * 1.2);
    double updated_fov = camera.getFovX();
    EXPECT_NE(updated_fov, initial_fov);

    camera.setHorizontalFov(initial_fov);
    EXPECT_DOUBLE_EQ(camera.getSensorWidth(), initial_sensor_width);
}

TEST(CameraTest, TestFocalLengthMinMax) {
    Camera camera;
    camera.setFocalLength(0.0);
    EXPECT_DOUBLE_EQ(camera.getFocalLength(), 0.01);

    camera.setFocalLength(1000.0);
    EXPECT_DOUBLE_EQ(camera.getFocalLength(), 0.8);

    camera.setFocalLength(50.0);
    EXPECT_DOUBLE_EQ(camera.getFocalLength(), 0.05);
}

TEST(CameraTest, TestApertureMinMax) {
    Camera camera;
    camera.setAperture(0.9);
    EXPECT_DOUBLE_EQ(camera.getAperture(), 1.0);

    camera.setAperture(23.0);
    EXPECT_DOUBLE_EQ(camera.getAperture(), 22.0);

    camera.setAperture(5.6);
    EXPECT_DOUBLE_EQ(camera.getAperture(), 5.6);
}

TEST(CameraTest, TestShutterSpeedMinMax) {
    Camera camera;
    camera.setShutterSpeed(1.0 / 10000.0);
    EXPECT_DOUBLE_EQ(camera.getShutterSpeed(), 1.0 / 8000.0);

    camera.setShutterSpeed(2.0);
    EXPECT_DOUBLE_EQ(camera.getShutterSpeed(), 1.0);

    camera.setShutterSpeed(1.0 / 500.0);
    EXPECT_DOUBLE_EQ(camera.getShutterSpeed(), 1.0 / 500.0);
}

TEST(CameraTest, TestFocusDistanceMinMax) {
    Camera camera;
    camera.setFocusDistance(0.1);
    EXPECT_DOUBLE_EQ(camera.getFocusDistance(), 0.3);

    camera.setFocusDistance(20000.0);
    EXPECT_DOUBLE_EQ(camera.getFocusDistance(), 10000.0);

    camera.setFocusDistance(5.0);
    EXPECT_DOUBLE_EQ(camera.getFocusDistance(), 5.0);
}

TEST(CameraTest, TestLensRadius) {
    Camera camera;
    camera.setFocalLength(56.0);
    camera.setAperture(2.8);
    EXPECT_DOUBLE_EQ(camera.getLensRadius(), 10 / 1000.0);
}
TEST(CameraTest, TestFarPlaneAndNearPlaneInteraction) {
    Camera camera;

    camera.setNearPlane(10.0);
    camera.setFarPlane(5.0);
    EXPECT_GE(camera.getFarPlane(), camera.getNearPlane());

    camera.setFarPlane(50.0);
    camera.setNearPlane(60.0);
    EXPECT_LE(camera.getNearPlane(), camera.getFarPlane());

    camera.setNearPlane(5.0);
    camera.setFarPlane(100.0);
    EXPECT_DOUBLE_EQ(camera.getNearPlane(), 5.0);
    EXPECT_DOUBLE_EQ(camera.getFarPlane(), 100.0);
}

TEST(CameraTest, CameraChangedObserver) {
    Camera camera;
    bool camera_changed_called = false;

    camera.getCameraChangedObserver().add([&]() { camera_changed_called = true; });

    camera.setHorizontalFov(60.0);
    EXPECT_TRUE(camera_changed_called);

    camera_changed_called = false;
    camera.setSensorWidth(35.0);
    EXPECT_TRUE(camera_changed_called);

    camera_changed_called = false;
    camera.setFocalLength(50.0);
    EXPECT_TRUE(camera_changed_called);
}

