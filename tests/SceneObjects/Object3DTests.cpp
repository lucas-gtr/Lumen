#include "SceneObjects/Object3D.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Surface/MaterialManager.hpp"

#include <gtest/gtest.h>

TEST(Object3DTest, MeshConstructorTest) {
    Mesh mesh; 
    Object3D obj(mesh);
    
    EXPECT_EQ(obj.getMesh(), mesh);
}

TEST(Object3DTest, SetMeshTest) {
    Mesh mesh1;

    Object3D obj;
    obj.setMesh(mesh1);

    EXPECT_EQ(obj.getMesh(), mesh1);
}

TEST(Object3DTest, SetMaterialTest) {
    Mesh mesh;
    Object3D obj(mesh);
    auto material = Material();
    obj.setMaterial(&material);

    EXPECT_EQ(obj.getMaterial(), &material);
}

TEST(Object3DTest, BoundsTestZero) {
    Mesh mesh;
    Object3D obj(mesh);

    linalg::Vec3d minBound = obj.getMinBound();
    linalg::Vec3d maxBound = obj.getMaxBound();

    EXPECT_EQ(minBound, linalg::Vec3d(0.0));
    EXPECT_EQ(maxBound, linalg::Vec3d(0.0));
}

TEST(Object3DTest, BoundsTestCube) {
    Mesh mesh = CubeMeshBuilder(1.0).build();
    Object3D obj(mesh);

    linalg::Vec3d minBound = obj.getMinBound();
    linalg::Vec3d maxBound = obj.getMaxBound();

    EXPECT_EQ(minBound, linalg::Vec3d(-0.5, -0.5, -0.5));
    EXPECT_EQ(maxBound, linalg::Vec3d(0.5, 0.5, 0.5));
}

TEST(Object3DTest, BoundsTransformationTest) {
    Mesh mesh = CubeMeshBuilder(1.0).build();
    Object3D obj(mesh);

    linalg::Vec3d translation(1.0, 2.0, 3.0);
    obj.setPosition(translation);
    obj.setRotationY(45.0);
    obj.setScale(linalg::Vec3d(2.0, 2.0, 2.0));

    linalg::Vec3d minBound = obj.getMinBound();
    linalg::Vec3d maxBound = obj.getMaxBound();

    linalg::Vec3d expectedMinBound(1.0 - std::sqrt(8.0), 1.0, 3.0 - std::sqrt(8.0));
    linalg::Vec3d expectedMaxBound(1.0 + std::sqrt(8.0), 3.0, 3.0 + std::sqrt(8.0));
}

TEST(Object3DTest, MaterialChangedObserverTest) {
    Mesh mesh;
    Object3D obj(mesh);
    bool material_changed_called = false;

    obj.getMaterialChangedObserver().add([&](const Object3D* object) {
        material_changed_called = true;
    });

    auto material = Material();
    obj.setMaterial(&material);

    EXPECT_TRUE(material_changed_called);
}

TEST(Object3DTest, ObjectDeletedObserverTest) {
    Mesh mesh;
    Object3D* obj = new Object3D(mesh);
    bool object_deleted_called = false;

    obj->getObjectDeletedObserver().add([&](const Object3D* object) {
        object_deleted_called = true;
    });

    delete obj;

    EXPECT_TRUE(object_deleted_called);
}

TEST(Object3DTest, SetMaterialNullptrTest) {
    Mesh mesh;
    Object3D obj(mesh);
    
    EXPECT_EQ(obj.getMaterial(), MaterialManager::DefaultMaterial());

    obj.setMaterial(nullptr);
    EXPECT_EQ(obj.getMaterial(), MaterialManager::DefaultMaterial());
}