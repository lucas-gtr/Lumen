#include "Scene/Scene.hpp"
#include "Scene/Skybox.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Texture.hpp"
#include "BVH/BVHNode.hpp"

#include <gtest/gtest.h>

TEST(SceneTest, AddObject) {
  Scene scene;
  scene.addObject("1", std::make_unique<Object3D>(Mesh()));
  EXPECT_EQ(scene.getObjectList().size(), 1);
  EXPECT_NE(scene.getObjectList()[0], nullptr);

  Object3D* obj = scene.getObject("1");
  EXPECT_EQ(scene.getObjectName(obj), "1");
  EXPECT_EQ(scene.getObject("1"), obj);
}

TEST(SceneTest, AddExistingObject) {
  Scene scene;
  scene.addObject("object", std::make_unique<Object3D>(Mesh()));
  
  EXPECT_THROW(scene.addObject("object", std::make_unique<Object3D>(Mesh())), std::runtime_error);
}

TEST(SceneTest, RemoveObject) {
  Scene scene;
  scene.addObject("object", std::make_unique<Object3D>(Mesh()));
  
  EXPECT_EQ(scene.getObjectList().size(), 1);
  scene.removeObject("object");
  EXPECT_EQ(scene.getObjectList().size(), 0);
  EXPECT_EQ(scene.getObject("object"), nullptr);
}

TEST(SceneTest, AddLight) {
  Scene scene;
  scene.addLight("light", std::make_unique<DirectionalLight>());
  EXPECT_EQ(scene.getLightList().size(), 1);
  EXPECT_NE(scene.getLightList()[0], nullptr);

  Light* light = scene.getLight("light");
  EXPECT_EQ(scene.getLightName(light), "light");
  EXPECT_EQ(scene.getLight("light"), light);
}

TEST(SceneTest, AddExistingLight) {
  Scene scene;
  scene.addLight("light", std::make_unique<DirectionalLight>());
  
  EXPECT_THROW(scene.addLight("light", std::make_unique<DirectionalLight>()), std::runtime_error);
}

TEST(SceneTest, RemoveLight) {
  Scene scene;
  scene.addLight("light", std::make_unique<DirectionalLight>());
  
  EXPECT_EQ(scene.getLightList().size(), 1);
  scene.removeLight("light");
  EXPECT_EQ(scene.getLightList().size(), 0);
  EXPECT_EQ(scene.getLight("light"), nullptr);
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
  ColorRGB color = scene.getSkyboxColor(direction);
  EXPECT_EQ(color, ColorRGB(1.0, 0.0, 0.0)); 
}

TEST(SceneTest, BuildBVH) {
  Scene scene;
  scene.addObject("1", std::make_unique<Object3D>(Mesh()));
  scene.buildBVH();
  EXPECT_NE(scene.getBVHRoot(), nullptr);
  EXPECT_EQ(scene.getBVHRoot()->getLeafIndex(), 0);
}

TEST(SceneTest, ObjectAddedObserver) {
  Scene scene;
  bool object_added_called = false;
  scene.getObjectAddedObserver().add([&](Object3D* obj) { object_added_called = true; });
  
  scene.addObject("object", std::make_unique<Object3D>(Mesh()));
  EXPECT_TRUE(object_added_called);
}

TEST(SceneTest, LightAddedObserver) {
  Scene scene;
  bool light_added_called = false;
  scene.getLightAddedObserver().add([&](Light* light) { light_added_called = true; });
  
  scene.addLight("light", std::make_unique<DirectionalLight>());
  EXPECT_TRUE(light_added_called);
}

TEST(SceneTest, RenameObject) {
  Scene scene;
  scene.addObject("object", std::make_unique<Object3D>(Mesh()));
  
  Object3D* obj = scene.getObject("object");

  EXPECT_FALSE(scene.renameObject("non_existent_object", "new_object"));

  EXPECT_TRUE(scene.renameObject("object", "new_object"));
  EXPECT_EQ(scene.getObjectName(obj), "new_object");
  EXPECT_EQ(scene.getObject("object"), nullptr);
  EXPECT_EQ(scene.getObject("new_object"), obj);

  scene.addObject("another_object", std::make_unique<Object3D>(Mesh()));
  EXPECT_FALSE(scene.renameObject("new_object", "another_object"));
}

TEST(SceneTest, RenameLight) {
  Scene scene;
  scene.addLight("light", std::make_unique<DirectionalLight>());

  Light* light = scene.getLight("light");

  EXPECT_FALSE(scene.renameLight("non_existent_light", "new_light"));
  
  EXPECT_TRUE(scene.renameLight("light", "new_light"));
  EXPECT_EQ(scene.getLightName(light), "new_light");
  EXPECT_EQ(scene.getLight("light"), nullptr);
  EXPECT_EQ(scene.getLight("new_light"), light);

  scene.addLight("another_light", std::make_unique<DirectionalLight>());
  EXPECT_FALSE(scene.renameLight("new_light", "another_light"));
}

TEST(SceneTest, GetNonExistentObject) {
  Scene scene;
  EXPECT_EQ(scene.getObject("non_existent_object"), nullptr);

  Object3D* obj = new Object3D(Mesh());
  EXPECT_EQ(scene.getObjectName(obj), "");
}

TEST(SceneTest, GetNonExistentLight) {
  Scene scene;
  EXPECT_EQ(scene.getLight("non_existent_light"), nullptr);

  Light* light = new DirectionalLight();
  EXPECT_EQ(scene.getLightName(light), "");
}

TEST(SceneTest, SceneTest_GetAvailablObjectName) {
  Scene scene;
  std::string name = scene.getAvailableObjectName("object");
  EXPECT_EQ(name, "object");

  scene.addObject(name, std::make_unique<Object3D>(Mesh()));

  name = scene.getAvailableObjectName("object");
  EXPECT_EQ(name, "object_1");

  scene.addObject(name, std::make_unique<Object3D>(Mesh()));
  
  name = scene.getAvailableObjectName("object");
  EXPECT_EQ(name, "object_2");

  scene.removeObject("object");
  
  name = scene.getAvailableObjectName("object");
  EXPECT_EQ(name, "object");
}

TEST(SceneTest, SceneTest_GetAvailableLightName) {
  Scene scene;
  std::string name = scene.getAvailableLightName("light");
  EXPECT_EQ(name, "light");

  scene.addLight(name, std::make_unique<DirectionalLight>());

  name = scene.getAvailableLightName("light");
  EXPECT_EQ(name, "light_1");

  scene.addLight(name, std::make_unique<DirectionalLight>());
  
  name = scene.getAvailableLightName("light");
  EXPECT_EQ(name, "light_2");

  scene.removeLight("light");
  
  name = scene.getAvailableLightName("light");
  EXPECT_EQ(name, "light");
}