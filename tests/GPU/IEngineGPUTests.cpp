#include <gtest/gtest.h>
#include "GPU/IEngineGPU.hpp"

class TestEngineGPU : public IEngineGPU {
public:
    TestEngineGPU() : IEngineGPU() {}

    void render() override {}  // Implémentation vide pour satisfaire l'interface

    float getMoveSpeed() const { return cameraMoveSpeed(); }
    float getRotateSpeed() const { return cameraRotateSpeed(); }
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
  engine.setCameraRotateSpeed(0.5f);
  engine.setCameraZoomSpeed(0.5f);

  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), 0.5f);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), 0.5f);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), 0.5f);
}

TEST(IEngineGPUTest, ClampSpeedsTooLow) {
  TestEngineGPU engine;
  engine.setCameraMoveSpeed(0.0f);
  engine.setCameraRotateSpeed(0.0f);
  engine.setCameraZoomSpeed(-1.0f);

  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), MIN_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), MIN_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), MIN_CAMERA_MOVE_SPEED);
}

TEST(IEngineGPUTest, ClampSpeedsTooHigh) {
  TestEngineGPU engine;
  engine.setCameraMoveSpeed(100.0f);
  engine.setCameraRotateSpeed(50.0f);
  engine.setCameraZoomSpeed(20.0f);

  EXPECT_FLOAT_EQ(engine.getMoveSpeed(), MAX_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getRotateSpeed(), MAX_CAMERA_MOVE_SPEED);
  EXPECT_FLOAT_EQ(engine.getZoomSpeed(), MAX_CAMERA_MOVE_SPEED);
}
