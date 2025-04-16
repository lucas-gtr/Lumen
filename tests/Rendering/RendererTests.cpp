#include <gtest/gtest.h>
#include "Rendering/Renderer.hpp"
#include "Core/Camera.hpp"
#include "Core/Object3D.hpp"
#include "Core/Mesh.hpp"

class RendererTest : public ::testing::Test {
protected:
  Scene scene;
  RenderSettings settings;
  Camera camera;

  void SetUp() override {
    scene.setCamera(std::make_unique<Camera>());
    scene.setBackgroundColor(Eigen::Vector3d(0.1, 0.2, 0.3));

    settings.setWidth(2);
    settings.setHeight(2);
    settings.setChannelCount(3);
    settings.setSamplesPerPixel(1);
    settings.setNearPlane(0.1);
    settings.setFarPlane(100.0);
  }
};

TEST_F(RendererTest, ConstructorInitializesCorrectly) {
  Renderer renderer(&settings, &scene);

  EXPECT_EQ(&renderer.getRenderSettings(), &settings);
  EXPECT_EQ(&renderer.getScene(), &scene);
  ASSERT_NE(renderer.getFramebuffer(), nullptr);
}

TEST_F(RendererTest, IsReadyToRenderReturnsTrueWithCamera) {
  Renderer renderer(&settings, &scene);
  EXPECT_TRUE(renderer.isReadyToRender());
}

TEST_F(RendererTest, IsReadyToRenderReturnsFalseWithoutCamera) {
  scene.setCamera(nullptr);
  Renderer renderer(&settings, &scene);
  EXPECT_FALSE(renderer.isReadyToRender());
}

TEST_F(RendererTest, RenderFrameRendersSomething) {
  settings.setWidth(1);
  settings.setHeight(1);
  scene.setBackgroundColor(Eigen::Vector3d(1.0, 0.0, 0.0));
  Renderer renderer(&settings, &scene);

  renderer.renderFrame();

  renderer.getFramebuffer()->generateImage();
  const unsigned char* image = renderer.getFramebuffer()->getImage();

  EXPECT_EQ(image[0], 255);
  EXPECT_EQ(image[1], 0);
  EXPECT_EQ(image[2], 0);
}

TEST_F(RendererTest, FramebufferUpdatesWhenRenderSettingsChange) {
  Renderer renderer(&settings, &scene);

  EXPECT_EQ(renderer.getFramebuffer()->getWidth(), settings.getWidth());
  EXPECT_EQ(renderer.getFramebuffer()->getHeight(), settings.getHeight());

  settings.setWidth(4);
  settings.setHeight(4);
  renderer.setRenderSettings(&settings);

  EXPECT_EQ(renderer.getFramebuffer()->getWidth(), 4);
  EXPECT_EQ(renderer.getFramebuffer()->getHeight(), 4);
}



