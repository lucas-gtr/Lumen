#include <gtest/gtest.h>
#include "Surface/MaterialManager.hpp"
#include "Surface/Material.hpp"

TEST(MaterialManagerTest, AddMaterialAndAvoidDuplicates) {
    MaterialManager manager;

    manager.addMaterial("myMaterial");
    auto mat1 = manager.getMaterial("myMaterial");
    ASSERT_NE(mat1, nullptr);

    manager.addMaterial("myMaterial");
    auto mat2 = manager.getMaterial("myMaterial");
    EXPECT_EQ(mat1, mat2); 
}

TEST(MaterialManagerTest, GetMaterialReturnsNullptrIfNotFound) {
    MaterialManager manager;
    EXPECT_EQ(manager.getMaterial("nonexistent"), nullptr);
}

TEST(MaterialManagerTest, GetMaterialNameReturnsNameOrDefaultIfNotFound) {
    MaterialManager manager;
    std::string base_name = "material";

    manager.addMaterial(base_name);
    Material* mat1 = manager.getMaterial(base_name);
    EXPECT_EQ(manager.getMaterialName(mat1), base_name);

    Material mat;
    EXPECT_EQ(manager.getMaterialName(&mat), "Default");
}

TEST(MaterialManagerTest, RemoveMaterialRemovesMaterial) {
    MaterialManager manager;
    manager.addMaterial("matToRemove");
    ASSERT_NE(manager.getMaterial("matToRemove"), nullptr);

    manager.removeMaterial("matToRemove");
    EXPECT_EQ(manager.getMaterial("matToRemove"), nullptr);
}

TEST(MaterialManagerTest, RenameMaterialSuccessAndFail) {
    MaterialManager manager;
    manager.addMaterial("oldName");
    ASSERT_NE(manager.getMaterial("oldName"), nullptr);

    bool renamed = manager.renameMaterial("oldName", "newName");
    EXPECT_TRUE(renamed);
    EXPECT_EQ(manager.getMaterial("oldName"), nullptr);
    EXPECT_NE(manager.getMaterial("newName"), nullptr);

    EXPECT_FALSE(manager.renameMaterial("nonexistent", "anyName"));

    manager.addMaterial("existingName");
    EXPECT_FALSE(manager.renameMaterial("newName", "existingName"));

    EXPECT_FALSE(manager.renameMaterial("newName", ""));
}

TEST(MaterialManagerTest, GetAvailableMaterialNameGeneratesUniqueName) {
    MaterialManager manager;
    std::string base_name = manager.getAvailableMaterialName("base");
    EXPECT_EQ(base_name, base_name);
    manager.addMaterial(base_name);
    std::string uniqueName = manager.getAvailableMaterialName("base");
    EXPECT_NE(uniqueName, "base");

    manager.addMaterial(uniqueName);
    std::string uniqueName2 = manager.getAvailableMaterialName("base");
    EXPECT_NE(uniqueName2, "base");
    EXPECT_NE(uniqueName2, uniqueName);
}

TEST(MaterialManagerTest, ObserversAreTriggered) {
    MaterialManager manager;

    bool addedCalled = false;
    std::string addedName;
    manager.getMaterialAddedObserver().add([&](const std::string& name){
        addedCalled = true;
        addedName = name;
    });

    bool removedCalled = false;
    std::string removedName;
    manager.getMaterialRemovedObserver().add([&](const std::string& name){
        removedCalled = true;
        removedName = name;
    });

    bool renamedCalled = false;
    std::string oldNameCaptured, newNameCaptured;
    manager.getMaterialRenamedObserver().add([&](const std::string& oldName, const std::string& newName){
        renamedCalled = true;
        oldNameCaptured = oldName;
        newNameCaptured = newName;
    });

    manager.addMaterial("mat1");
    EXPECT_TRUE(addedCalled);
    EXPECT_EQ(addedName, "mat1");

    addedCalled = false;
    removedCalled = false;
    renamedCalled = false;

    manager.renameMaterial("mat1", "mat2");
    EXPECT_TRUE(renamedCalled);
    EXPECT_EQ(oldNameCaptured, "mat1");
    EXPECT_EQ(newNameCaptured, "mat2");

    renamedCalled = false;
    removedCalled = false;
    manager.removeMaterial("mat2");
    EXPECT_TRUE(removedCalled);
    EXPECT_EQ(removedName, "mat2");
}

TEST(MaterialManagerTest, DefaultMaterialProperties) {
    MaterialManager manager;
    auto defaultMaterial = MaterialManager::DefaultMaterial();
    ASSERT_NE(defaultMaterial, nullptr);

    EXPECT_EQ(defaultMaterial->getDiffuse({0, 0}), ColorRGB(1.0, 1.0, 1.0));
    EXPECT_EQ(defaultMaterial->getNormal({0, 0}), ColorRGB(0.5, 0.5, 1.0));
    EXPECT_DOUBLE_EQ(defaultMaterial->getRoughness({0, 0}), 0.5);
    EXPECT_DOUBLE_EQ(defaultMaterial->getMetalness({0, 0}), 0.0);
    EXPECT_DOUBLE_EQ(defaultMaterial->getTransmission({0, 0}), 0.0);
    EXPECT_DOUBLE_EQ(defaultMaterial->getIndexOfRefraction(), 1.0);
}

TEST(MaterialManagerTest, GetAllMaterialsName) {
    MaterialManager manager;
    manager.addMaterial("mat1");
    manager.addMaterial("mat2");
    manager.addMaterial("mat3");

    auto names = manager.getAllMaterialsName();
    EXPECT_EQ(names.size(), 3);
    EXPECT_TRUE(std::find(names.begin(), names.end(), "mat1") != names.end());
    EXPECT_TRUE(std::find(names.begin(), names.end(), "mat2") != names.end());
    EXPECT_TRUE(std::find(names.begin(), names.end(), "mat3") != names.end());
}
