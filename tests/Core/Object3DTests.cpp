#include <gtest/gtest.h>
#include "Core/Object3D.hpp"
#include "Core/Mesh.hpp"

TEST(Object3DTest, DefaultConstructorTest) {
    Mesh mesh; 
    Object3D obj(mesh);
    
    EXPECT_EQ(obj.getMesh(), mesh);
}

TEST(Object3DTest, SetMeshTest) {
    Mesh mesh1;
    Mesh mesh2;

    Object3D obj(mesh1);
    obj.setMesh(mesh2);

    EXPECT_EQ(obj.getMesh(), mesh2);
}

TEST(Object3DTest, CopyConstructorTest) {
    Mesh mesh;
    Object3D obj1(mesh);
    Object3D obj2 = obj1;

    EXPECT_EQ(obj1.getMesh(), obj2.getMesh());
}

TEST(Object3DTest, AssignmentOperatorTest) {
    Mesh mesh1;
    Mesh mesh2;

    Object3D obj1(mesh1);
    Object3D obj2(mesh2);
    obj1 = obj2;

    EXPECT_EQ(obj1.getMesh(), obj2.getMesh());
}
