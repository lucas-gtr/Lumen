#include "Scene/Scene.hpp"
#include "Lighting/DirectionalLight.hpp"

#include <gtest/gtest.h>

TEST(SceneTest, AddObject) {
  Scene scene;
  scene.addObject(std::make_unique<Object3D>(Mesh()));
  EXPECT_EQ(scene.getObjectList().size(), 1);
  EXPECT_NE(scene.getObjectList()[0].get(), nullptr);
}

TEST(SceneTest, AddLight) {
  Scene scene;
  scene.addLight(std::make_unique<DirectionalLight>());
  EXPECT_EQ(scene.getLightList().size(), 1);
  EXPECT_NE(scene.getLightList()[0].get(), nullptr);
}

TEST(SceneTest, SetCamera) {
  Scene scene;
  scene.setCamera(std::make_unique<Camera>());
  EXPECT_NE(scene.getCamera(), nullptr);
}

TEST(SceneTest, GetSkyboxColor) {
  Scene scene;
  std::shared_ptr<Texture> texture = std::make_shared<Texture>(ColorRGB(1.0, 0.0, 0.0));
  scene.setSkybox(texture);
  Eigen::Vector3d direction(1.0, 0.0, 0.0);
  ColorRGBA color = scene.getSkyboxColor(direction);
  EXPECT_EQ(color, ColorRGBA(1.0, 0.0, 0.0, 1.0)); // Assuming default color is white
}

TEST(SceneTest, BuildBVH) {
  Scene scene;
  scene.addObject(std::make_unique<Object3D>(Mesh()));
  scene.buildBVH();
  EXPECT_NE(scene.getBVHRoot(), nullptr);
  EXPECT_EQ(scene.getBVHRoot()->getLeafIndex(), 0);
}