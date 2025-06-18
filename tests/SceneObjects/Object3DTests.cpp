#include "SceneObjects/Object3D.hpp"
#include "Geometry/CubeMeshBuilder.hpp"

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

    lin::Vec3d minBound = obj.getMinBound();
    lin::Vec3d maxBound = obj.getMaxBound();

    EXPECT_EQ(minBound, lin::Vec3d(0.0));
    EXPECT_EQ(maxBound, lin::Vec3d(0.0));
}

TEST(Object3DTest, BoundsTestCube) {
    Mesh mesh = CubeMeshBuilder(1.0).build();
    Object3D obj(mesh);

    lin::Vec3d minBound = obj.getMinBound();
    lin::Vec3d maxBound = obj.getMaxBound();

    EXPECT_EQ(minBound, lin::Vec3d(-0.5, -0.5, -0.5));
    EXPECT_EQ(maxBound, lin::Vec3d(0.5, 0.5, 0.5));
}

TEST(Object3DTest, BoundsTransformationTest) {
    Mesh mesh = CubeMeshBuilder(1.0).build();
    Object3D obj(mesh);

    lin::Vec3d translation(1.0, 2.0, 3.0);
    obj.setPosition(translation);
    obj.setRotationY(45.0);
    obj.setScale(lin::Vec3d(2.0, 2.0, 2.0));

    lin::Vec3d minBound = obj.getMinBound();
    lin::Vec3d maxBound = obj.getMaxBound();

    lin::Vec3d expectedMinBound(1.0 - std::sqrt(8.0), 1.0, 3.0 - std::sqrt(8.0));
    lin::Vec3d expectedMaxBound(1.0 + std::sqrt(8.0), 3.0, 3.0 + std::sqrt(8.0));
}
