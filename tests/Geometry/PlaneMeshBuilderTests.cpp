#include "Geometry/PlaneMeshBuilder.hpp"

#include <gtest/gtest.h>

TEST(PlaneMeshBuilderTest, BuildVertices) {
    PlaneMeshBuilder builder(10.0, 20.0);
    Mesh mesh = builder.build();
    
    EXPECT_EQ(mesh.getVertex(0).position.x, -5.0);
    EXPECT_EQ(mesh.getVertex(0).position.y, 0.0);
    EXPECT_EQ(mesh.getVertex(0).position.z, -10.0);

    EXPECT_EQ(mesh.getVertex(1).position.x, 5.0);
    EXPECT_EQ(mesh.getVertex(1).position.y, 0.0);
    EXPECT_EQ(mesh.getVertex(1).position.z, -10.0);

    EXPECT_EQ(mesh.getVertex(2).position.x, 5.0);
    EXPECT_EQ(mesh.getVertex(2).position.y, 0.0);
    EXPECT_EQ(mesh.getVertex(2).position.z, 10.0);

    EXPECT_EQ(mesh.getVertex(3).position.x, -5.0);
    EXPECT_EQ(mesh.getVertex(3).position.y, 0.0);
    EXPECT_EQ(mesh.getVertex(3).position.z, 10.0);
}

TEST(PlaneMeshBuilderTest, BuildFaceIndices) {
    PlaneMeshBuilder builder(10.0, 20.0);
    Mesh mesh = builder.build();
    
    EXPECT_EQ(mesh.getFaces().size(), 2);
    
    EXPECT_EQ(mesh.getFaces()[0].vertex_indices[0], 0);
    EXPECT_EQ(mesh.getFaces()[0].vertex_indices[1], 2);
    EXPECT_EQ(mesh.getFaces()[0].vertex_indices[2], 1);

    EXPECT_EQ(mesh.getFaces()[1].vertex_indices[0], 0);
    EXPECT_EQ(mesh.getFaces()[1].vertex_indices[1], 3);
    EXPECT_EQ(mesh.getFaces()[1].vertex_indices[2], 2);
}

TEST(PlaneMeshBuilderTest, BuildVertexNormals) {
    PlaneMeshBuilder builder(10.0, 20.0);
    Mesh mesh = builder.build();

    for (int i = 0; i < 4; ++i) {
        EXPECT_DOUBLE_EQ(mesh.getVertex(i).normal.x, 0.0);
        EXPECT_DOUBLE_EQ(mesh.getVertex(i).normal.y, 1.0);
        EXPECT_DOUBLE_EQ(mesh.getVertex(i).normal.z, 0.0);
    }
}

TEST(PlaneMeshBuilderTest, BuildVertexUVs) {
    PlaneMeshBuilder builder(10.0, 20.0);
    Mesh mesh = builder.build();

    EXPECT_DOUBLE_EQ(mesh.getVertex(0).uv_coord.u, 0.0);
    EXPECT_DOUBLE_EQ(mesh.getVertex(0).uv_coord.v, 0.0);

    EXPECT_DOUBLE_EQ(mesh.getVertex(1).uv_coord.u, 10.0);
    EXPECT_DOUBLE_EQ(mesh.getVertex(1).uv_coord.v, 0.0);

    EXPECT_DOUBLE_EQ(mesh.getVertex(2).uv_coord.u, 10.0);
    EXPECT_DOUBLE_EQ(mesh.getVertex(2).uv_coord.v, 20.0);

    EXPECT_DOUBLE_EQ(mesh.getVertex(3).uv_coord.u, 0.0);
    EXPECT_DOUBLE_EQ(mesh.getVertex(3).uv_coord.v, 20.0);
}

TEST(PlaneMeshBuilderTest, BuildInvalidSize) {
    PlaneMeshBuilder builder1(-10.0, 20.0);
    Mesh mesh1 = builder1.build();

    EXPECT_EQ(mesh1.getVertices().size(), 0);
    EXPECT_EQ(mesh1.getFaces().size(), 0);

    PlaneMeshBuilder builder2(10.0, -20.0);
    Mesh mesh2 = builder2.build();

    EXPECT_EQ(mesh2.getVertices().size(), 0);
    EXPECT_EQ(mesh2.getFaces().size(), 0);
}
