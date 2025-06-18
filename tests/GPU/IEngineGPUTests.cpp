#include <gtest/gtest.h>
#include "GPU/IEngineGPU.hpp"

class TestEngineGPU : public IEngineGPU {
public:
    TestEngineGPU() : IEngineGPU() {}

    float getMoveSpeed() const { return cameraMoveSpeed(); }
    float getRotateSpeed() const { return cameraRotationSpeed(); }
    float getZoomSpeed() const { return cameraZoomSpeed(); }
};

TEST(IEngineGPUTest, DefaultSpeeds) {
  TestEngineGPU engine;
  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), DEFAULT_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), DEFAULT_CAMERA_ROTATE_SPEED);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), DEFAULT_CAMERA_ZOOM_SPEED);
}

TEST(IEngineGPUTest, SetValidSpeeds) {
  TestEngineGPU engine;
  engine.setCameraMoveSpeed(0.5f);
  engine.setCameraRotationSpeed(0.5f);
  engine.setCameraZoomSpeed(0.5f);

  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), 0.5f);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), 0.5f);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), 0.5f);
}

TEST(IEngineGPUTest, ClampSpeedsTooLow) {
  TestEngineGPU engine;
  engine.setCameraMoveSpeed(0.0f);
  engine.setCameraRotationSpeed(0.0f);
  engine.setCameraZoomSpeed(-1.0f);

  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), MIN_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), MIN_CAMERA_ROTATION_SPEED);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), MIN_CAMERA_ZOOM_SPEED);
}

TEST(IEngineGPUTest, ClampSpeedsTooHigh) {
  TestEngineGPU engine;
  engine.setCameraMoveSpeed(100.0f);
  engine.setCameraRotationSpeed(50.0f);
  engine.setCameraZoomSpeed(200.0f);

  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), MAX_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), MAX_CAMERA_ROTATION_SPEED);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), MAX_CAMERA_ZOOM_SPEED);
}
