#include <gtest/gtest.h>
#include "Rendering/Renderer.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "PostProcessing/ToneMapping/None.hpp"
#include "Core/ColorUtils.hpp"

class RendererTest : public ::testing::Test {
protected:
  Scene scene;
  RenderSettings settings;

  void SetUp() override {
    settings.setWidth(2);
    settings.setHeight(2);
    settings.setChannelCount(3);
    settings.setSamplesPerPixel(1);
  }
};

TEST_F(RendererTest, ConstructorInitializesCorrectly) {
  Renderer renderer(&scene, &settings);

  EXPECT_EQ(&renderer.getRenderSettings(), &settings);
  EXPECT_EQ(&renderer.getScene(), &scene);
  ASSERT_NE(renderer.getFramebuffer(), nullptr);
}

TEST_F(RendererTest, IsReadyToRenderReturnsTrueWithCamera) {
  Renderer renderer(&scene, &settings);
  EXPECT_TRUE(renderer.isReadyToRender());
}

TEST_F(RendererTest, RenderFrameRendersSomething) {
  settings.setWidth(1);
  settings.setHeight(1);
  Renderer renderer(&scene, &settings);

  renderer.renderFrame();

  const double* image = renderer.getFramebuffer()->getFramebuffer();

  EXPECT_NEAR(image[0], 0.65, 0.001);
  EXPECT_NEAR(image[1], 0.65, 0.001);
  EXPECT_NEAR(image[2], 0.9, 0.001);
}

TEST_F(RendererTest, FramebufferUpdatesWhenRenderSettingsChange) {
  Renderer renderer(&scene, &settings);

  renderer.renderFrame();
  EXPECT_EQ(renderer.getFramebuffer()->getWidth(), settings.getWidth());
  EXPECT_EQ(renderer.getFramebuffer()->getHeight(), settings.getHeight());

  settings.setWidth(4);
  settings.setHeight(4);
  renderer.renderFrame();

  EXPECT_EQ(renderer.getFramebuffer()->getWidth(), 4);
  EXPECT_EQ(renderer.getFramebuffer()->getHeight(), 4);
}

TEST_F(RendererTest, RenderInfluencedByLight) {
  auto light = std::make_unique<DirectionalLight>();
  light->setDirection(linalg::Vec3d(0.0, 0.0, -1.0));
  light->setColor({0.9, 0.0, 0.0});
  light->setIntensity(1.0);
  scene.addLight("light", std::move(light));

  CubeMeshBuilder cube_builder(5.0);
  auto cube_mesh = cube_builder.build();
  auto cube_object = std::make_unique<Object3D>(cube_mesh);
  cube_object->setPosition(linalg::Vec3d(0.0, 0.0, -7.0));
  
  Texture texture;
  texture.setValue(ColorRGB(0.5, 0.3, 0.9));
  auto material = Material();
  material.setDiffuseTexture(&texture);
  cube_object->setMaterial(&material);
  scene.addObject("1", std::move(cube_object));

  scene.getCamera()->setPosition(linalg::Vec3d(0.0, 0.0, 0.0));
  scene.getCamera()->setRotation(linalg::Vec3d(0.0, 0.0, 0.0));
  scene.getCamera()->setHorizontalFov(10.0);

  settings.setWidth(1);
  settings.setHeight(1);
  Renderer renderer(&scene, &settings);

  renderer.renderFrame();

  const double* image = renderer.getFramebuffer()->getFramebuffer();
  double expected_red_value = 0.45;
  convertToSRGBSpace(expected_red_value);

  EXPECT_NEAR(image[0], expected_red_value, 0.001);
  EXPECT_NEAR(image[1], 0.0, 0.001);
  EXPECT_NEAR(image[2], 0.0, 0.001);
}

TEST_F(RendererTest, ShadowRayBlockedByObject) {
  auto light = std::make_unique<DirectionalLight>();
  light->setDirection(linalg::Vec3d(0.0, 0.0, -1.0));
  light->setColor({0.9, 0.0, 0.0});
  light->setIntensity(1.0);
  scene.addLight("light", std::move(light));

  CubeMeshBuilder cube_builder(5.0);
  auto cube_mesh = cube_builder.build();
  auto cube_object = std::make_unique<Object3D>(cube_mesh);
  cube_object->setPosition(linalg::Vec3d(0.0, 0.0, -7.0));

  CubeMeshBuilder cube_builder2(6.0);
  auto cube_mesh2 = cube_builder2.build();
  auto cube_object2 = std::make_unique<Object3D>(cube_mesh2);
  cube_object2->setPosition(linalg::Vec3d(0.0, 0.0, 6.5));
  
  Texture texture;
  texture.setValue(ColorRGB(0.5, 0.3, 0.9));
  auto material = Material();
  material.setDiffuseTexture(&texture);
  
  cube_object->setMaterial(&material);
  cube_object2->setMaterial(&material);

  scene.addObject("1", std::move(cube_object));
  scene.addObject("2", std::move(cube_object2));

  scene.getCamera()->setPosition(linalg::Vec3d(0.0, 0.0, 0.0));
  scene.getCamera()->setRotation(linalg::Vec3d(0.0, 0.0, 0.0));
  scene.getCamera()->setHorizontalFov(10.0);

  settings.setWidth(1);
  settings.setHeight(1);
  Renderer renderer(&scene, &settings);

  renderer.renderFrame();

  const double* image = renderer.getFramebuffer()->getFramebuffer();

  EXPECT_NEAR(image[0], 0.0, 0.001);
  EXPECT_NEAR(image[1], 0.0, 0.001);
  EXPECT_NEAR(image[2], 0.0, 0.001);
}



