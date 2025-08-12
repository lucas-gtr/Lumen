#include <gtest/gtest.h>

#include "Core/Color.hpp"
#include "Rendering/Renderer.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "PostProcessing/ToneMapping/None.hpp"

class RendererTest : public ::testing::Test {
protected:
  Scene scene;
  RenderSettings settings;

  void SetUp() override {
    settings.setWidth(2);
    settings.setHeight(2);
    settings.setSamplesPerPixel(1);
  }
};

TEST_F(RendererTest, ConstructorInitializesCorrectly) {
  Renderer renderer(&settings);

  EXPECT_EQ(&renderer.getRenderSettings(), &settings);
  EXPECT_EQ(&renderer.getScene(), nullptr);
  ASSERT_NE(renderer.getFramebuffer(), nullptr);
}

TEST_F(RendererTest, IsReadyToRenderReturnsTrueWithCamera) {
  Renderer renderer(&settings);
  renderer.setScene(&scene);
  
  EXPECT_TRUE(renderer.isReadyToRender());
}

TEST_F(RendererTest, RenderFrameRendersSomething) {
  settings.setWidth(1);
  settings.setHeight(1);
  Renderer renderer(&settings);
  renderer.setScene(&scene);

  Texture texture = Texture();
  texture.setValue(ColorRGB(0.65, 0.65, 0.9));
  texture.setColorSpace(ColorSpace::LINEAR);
  
  scene.setSkybox(&texture);

  renderer.renderFrame();
  const double* image = renderer.getFramebuffer()->getFramebuffer();

  EXPECT_NEAR(image[0], 0.65, 0.001);
  EXPECT_NEAR(image[1], 0.65, 0.001);
  EXPECT_NEAR(image[2], 0.9, 0.001);
}

TEST_F(RendererTest, FramebufferUpdatesWhenRenderSettingsChange) {
  Renderer renderer(&settings);
  renderer.setScene(&scene);

  renderer.renderFrame();
  EXPECT_EQ(renderer.getFramebuffer()->getWidth(), settings.getWidth());
  EXPECT_EQ(renderer.getFramebuffer()->getHeight(), settings.getHeight());

  settings.setWidth(4);
  settings.setHeight(4);
  renderer.renderFrame();

  EXPECT_EQ(renderer.getFramebuffer()->getWidth(), 4);
  EXPECT_EQ(renderer.getFramebuffer()->getHeight(), 4);
}



