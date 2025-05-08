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
    std::shared_ptr<Material> material = std::make_shared<Material>();
    obj.setMaterial(material);

    EXPECT_EQ(obj.getMaterial(), material);
}

TEST(Object3DTest, CopyConstructorTest) {
    Mesh mesh;
    Object3D obj1(mesh);
    std::shared_ptr<Material> material = std::make_shared<Material>();
    obj1.setMaterial(material);

    Object3D obj2 = obj1;

    Mesh meshTest1 = obj1.getMesh();
    Mesh meshTest2 = obj2.getMesh();

    EXPECT_EQ(obj1.getMesh(), obj2.getMesh());
    EXPECT_NE(&meshTest1, &meshTest2);
    EXPECT_EQ(obj1.getMaterial(), obj2.getMaterial());
}

TEST(Object3DTest, AssignmentOperatorTest) {
    Mesh mesh1;
    Mesh mesh2;

    Object3D obj1(mesh1);
    Object3D obj2(mesh2);
    obj1 = obj2;

    EXPECT_EQ(obj1.getMesh(), obj2.getMesh());
}

TEST(Object3DTest, BoundsTestZero) {
    Mesh mesh;
    Object3D obj(mesh);

    Eigen::Vector3d minBound = obj.getMinBound();
    Eigen::Vector3d maxBound = obj.getMaxBound();

    EXPECT_EQ(minBound, Eigen::Vector3d::Zero());
    EXPECT_EQ(maxBound, Eigen::Vector3d::Zero());
}

TEST(Object3DTest, BoundsTestCube) {
    Mesh mesh = CubeMeshBuilder(1.0).build();
    Object3D obj(mesh);

    Eigen::Vector3d minBound = obj.getMinBound();
    Eigen::Vector3d maxBound = obj.getMaxBound();

    EXPECT_EQ(minBound, Eigen::Vector3d(-0.5, -0.5, -0.5));
    EXPECT_EQ(maxBound, Eigen::Vector3d(0.5, 0.5, 0.5));
}

TEST(Object3DTest, BoundsTransformationTest) {
    Mesh mesh = CubeMeshBuilder(1.0).build();
    Object3D obj(mesh);

    Eigen::Vector3d translation(1.0, 2.0, 3.0);
    obj.setPosition(translation);
    obj.setRotationY(45.0);
    obj.setScale(Eigen::Vector3d(2.0, 2.0, 2.0));

    Eigen::Vector3d minBound = obj.getMinBound();
    Eigen::Vector3d maxBound = obj.getMaxBound();

    Eigen::Vector3d expectedMinBound(1.0 - std::sqrt(8.0), 1.0, 3.0 - std::sqrt(8.0));
    Eigen::Vector3d expectedMaxBound(1.0 + std::sqrt(8.0), 3.0, 3.0 + std::sqrt(8.0));
}