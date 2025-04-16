#include <gtest/gtest.h>
#include "Core/Scene.hpp"
#include "Core/Object3D.hpp"
#include "Core/Camera.hpp"

TEST(SceneTest, AddObject) {
  Scene scene;
  scene.addObject(std::make_unique<Object3D>(Mesh()));
  EXPECT_EQ(scene.getObjectList().size(), 1);
  EXPECT_NE(scene.getObjectList()[0].get(), nullptr);
}

TEST(SceneTest, SetCamera) {
  Scene scene;
  scene.setCamera(std::make_unique<Camera>());
  EXPECT_NE(scene.getCamera(), nullptr);
}