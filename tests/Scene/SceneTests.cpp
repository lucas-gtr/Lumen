#include "Scene/Scene.hpp"
#include "Lighting/DirectionalLight.hpp"

#include <gtest/gtest.h>

TEST(SceneTest, AddObject) {
  Scene scene;
  scene.addObject("1", std::make_unique<Object3D>(Mesh()));
  EXPECT_EQ(scene.getObjectList().size(), 1);
  EXPECT_NE(scene.getObjectList()[0], nullptr);
}

TEST(SceneTest, AddLight) {
  Scene scene;
  scene.addLight("light", std::make_unique<DirectionalLight>());
  EXPECT_EQ(scene.getLightList().size(), 1);
  EXPECT_NE(scene.getLightList()[0], nullptr);
}

TEST(SceneTest, GetCamera) {
  Scene scene;
  EXPECT_NE(scene.getCamera(), nullptr);
}

TEST(SceneTest, GetSkybox) {
  Scene scene;
  Texture texture;
  texture.setValue(ColorRGB(0.5, 0.5, 0.5));
  scene.setSkybox(&texture);
  EXPECT_NE(scene.getSkybox(), nullptr);
}

TEST(SceneTest, GetSkyboxColor) {
  Scene scene;
  Texture texture;
  texture.setValue(ColorRGB(1.0, 0.0, 0.0));
  scene.setSkybox(&texture);
  linalg::Vec3d direction(1.0, 0.0, 0.0);
  ColorRGBA color = scene.getSkyboxColor(direction);
  EXPECT_EQ(color, ColorRGBA(1.0, 0.0, 0.0, 1.0)); 
}

TEST(SceneTest, BuildBVH) {
  Scene scene;
  scene.addObject("1", std::make_unique<Object3D>(Mesh()));
  scene.buildBVH();
  EXPECT_NE(scene.getBVHRoot(), nullptr);
  EXPECT_EQ(scene.getBVHRoot()->getLeafIndex(), 0);
}