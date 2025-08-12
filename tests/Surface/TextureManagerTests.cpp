#include <gtest/gtest.h>
#include "Surface/TextureManager.hpp"
#include "Surface/Texture.hpp"

TEST(TextureManagerTest, AddTextureAndAvoidDuplicates) {
    TextureManager manager;

    manager.addTexture("myTexture");
    auto tex1 = manager.getTexture("myTexture");
    ASSERT_NE(tex1, nullptr);

    manager.addTexture("myTexture");
    auto tex2 = manager.getTexture("myTexture");
    EXPECT_EQ(tex1, tex2); 
}

TEST(TextureManagerTest, GetTextureReturnsNullptrIfNotFound) {
    TextureManager manager;
    EXPECT_EQ(manager.getTexture("nonexistent"), nullptr);
}

TEST(TextureManagerTest, GetTextureNameReturnsNameOrDefaultIfNotFound) {
    TextureManager manager;
    std::string base_name = "texture";

    manager.addTexture(base_name);
    Texture* tex1 = manager.getTexture(base_name);
    EXPECT_EQ(manager.getTextureName(tex1), base_name);

    Texture tex;
    EXPECT_EQ(manager.getTextureName(&tex), "Default");
}

TEST(TextureManagerTest, RemoveTextureRemovesTexture) {
    TextureManager manager;
    manager.addTexture("texToRemove");
    ASSERT_NE(manager.getTexture("texToRemove"), nullptr);

    manager.removeTexture("texToRemove");
    EXPECT_EQ(manager.getTexture("texToRemove"), nullptr);
}

TEST(TextureManagerTest, RenameTextureSuccessAndFail) {
    TextureManager manager;
    manager.addTexture("oldName");
    ASSERT_NE(manager.getTexture("oldName"), nullptr);

    bool renamed = manager.renameTexture("oldName", "newName");
    EXPECT_TRUE(renamed);
    EXPECT_EQ(manager.getTexture("oldName"), nullptr);
    EXPECT_NE(manager.getTexture("newName"), nullptr);

    EXPECT_FALSE(manager.renameTexture("nonexistent", "anyName"));

    manager.addTexture("existingName");
    EXPECT_FALSE(manager.renameTexture("newName", "existingName"));

    EXPECT_FALSE(manager.renameTexture("newName", ""));
}

TEST(TextureManagerTest, GetAvailableTextureNameGeneratesUniqueName) {
    TextureManager manager;
    std::string base_name = manager.getAvailableTextureName("base");
    EXPECT_EQ(base_name,base_name);
    manager.addTexture(base_name);
    std::string uniqueName = manager.getAvailableTextureName("base");
    EXPECT_NE(uniqueName, "base");

    manager.addTexture(uniqueName);
    std::string uniqueName2 = manager.getAvailableTextureName("base");
    EXPECT_NE(uniqueName2, "base");
    EXPECT_NE(uniqueName2, uniqueName);
}

TEST(TextureManagerTest, ObserversAreTriggered) {
    TextureManager manager;

    bool addedCalled = false;
    std::string addedName;
    manager.getTextureAddedObserver().add([&](const std::string& name){
        addedCalled = true;
        addedName = name;
    });

    bool removedCalled = false;
    std::string removedName;
    manager.getTextureRemovedObserver().add([&](const std::string& name){
        removedCalled = true;
        removedName = name;
    });

    bool renamedCalled = false;
    std::string oldNameCaptured, newNameCaptured;
    manager.getTextureRenamedObserver().add([&](const std::string& oldName, const std::string& newName){
        renamedCalled = true;
        oldNameCaptured = oldName;
        newNameCaptured = newName;
    });

    manager.addTexture("tex1");
    EXPECT_TRUE(addedCalled);
    EXPECT_EQ(addedName, "tex1");

    addedCalled = false;
    removedCalled = false;
    renamedCalled = false;

    manager.renameTexture("tex1", "tex2");
    EXPECT_TRUE(renamedCalled);
    EXPECT_EQ(oldNameCaptured, "tex1");
    EXPECT_EQ(newNameCaptured, "tex2");

    renamedCalled = false;
    removedCalled = false;
    manager.removeTexture("tex2");
    EXPECT_TRUE(removedCalled);
    EXPECT_EQ(removedName, "tex2");
}

TEST(TextureManagerTest, DefaultTexturesProperties) {
    Texture* skybox = TextureManager::DefaultSkyboxTexture();
    ASSERT_NE(skybox, nullptr);

    auto skyboxValue = skybox->getValue3d({0.0, 0.0});
    EXPECT_FLOAT_EQ(skyboxValue.r, 0.f);
    EXPECT_FLOAT_EQ(skyboxValue.g, 0.f);
    EXPECT_FLOAT_EQ(skyboxValue.b, 0.f);

    Texture* normal = TextureManager::DefaultNormalTexture();
    ASSERT_NE(normal, nullptr);
    auto normalValue = normal->getValue3d({0.0, 0.0});

    EXPECT_GE(normalValue.r, 0.f);
    EXPECT_GE(normalValue.g, 0.f);
    EXPECT_GE(normalValue.b, 0.f);

    Texture* diffuse = TextureManager::DefaultDiffuseTexture();
    ASSERT_NE(diffuse, nullptr);
    auto diffuseValue = diffuse->getValue3d({0.0, 0.0});
    EXPECT_GE(diffuseValue.r, 0.f);
    EXPECT_GE(diffuseValue.g, 0.f);
    EXPECT_GE(diffuseValue.b, 0.f);

    Texture* transmission = TextureManager::DefaultWhiteTexture();
    ASSERT_NE(transmission, nullptr);
    auto transValue = transmission->getValue3d({0.0, 0.0});
    EXPECT_GE(transValue.r, 0.f);
    EXPECT_GE(transValue.g, 0.f);
    EXPECT_GE(transValue.b, 0.f);

    Texture* roughness = TextureManager::DefaultMidGrayTexture();
    ASSERT_NE(roughness, nullptr);
    auto roughValue = roughness->getValue1d({0.0, 0.0});
    EXPECT_NEAR(roughValue, 0.5f, 1e-6);

    Texture* metallic = TextureManager::DefaultBlackTexture();
    ASSERT_NE(metallic, nullptr);
    auto metalValue = metallic->getValue1d({0.0, 0.0});
    EXPECT_EQ(metalValue, 0.0f);
}

TEST(TextureManagerTest, GetAllTexturesNameReturnsAllNames) {
    TextureManager manager;
    manager.addTexture("t1");
    manager.addTexture("t2");
    manager.addTexture("t3");

    auto names = manager.getAllTexturesName();
    EXPECT_EQ(names.size(), 3);
    EXPECT_NE(std::find(names.begin(), names.end(), "t1"), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), "t2"), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), "t3"), names.end());
}
