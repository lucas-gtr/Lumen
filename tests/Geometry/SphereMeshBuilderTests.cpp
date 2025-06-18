#include "Geometry/SphereMeshBuilder.hpp"

#include "gtest/gtest.h"

// TEST(SphereMeshBuilderTest, TestBuildMeshWithZeroRings) {
//     SphereMeshBuilder builder(1.0, 10, 0);
//     Mesh mesh = builder.build();

//     ASSERT_EQ(mesh.getVertices().size(), 2);
//     ASSERT_EQ(mesh.getFaces().size(), 0);
// }

// TEST(SphereMeshBuilderTest, TestBuildMeshWithZeroSegments) {
//     SphereMeshBuilder builder(1.0, 0, 10);
//     Mesh mesh = builder.build();

//     ASSERT_EQ(mesh.getVertices().size(), 11);
//     ASSERT_EQ(mesh.getFaces().size(), 0);
// }

TEST(SphereMeshBuilderTest, TestBuildMeshWithValidParams) {
    SphereMeshBuilder builder(1.0, 10, 10);
    Mesh mesh = builder.build();

    ASSERT_GT(mesh.getVertices().size(), 0);
    ASSERT_GT(mesh.getFaces().size(), 0);
}

TEST(SphereMeshBuilderTest, TestVertexPositionAndNormal) {
    SphereMeshBuilder builder(1.0, 10, 10);
    Mesh mesh = builder.build();

    const auto& vertices = mesh.getVertices();
    const Vertex& vertex = vertices[0];

    ASSERT_NEAR(vertex.position.x, 0.0, 1e-6);
    ASSERT_NEAR(vertex.position.y, 1.0, 1e-6);
    ASSERT_NEAR(vertex.position.z, 0.0, 1e-6);

    ASSERT_NEAR(vertex.normal.x, 0.0, 1e-6);
    ASSERT_NEAR(vertex.normal.y, 1.0, 1e-6);
    ASSERT_NEAR(vertex.normal.z, 0.0, 1e-6);
}

TEST(SphereMeshBuilderTest, TestUVCoord) {
    SphereMeshBuilder builder(1.0, 10, 10);
    Mesh mesh = builder.build();

    const auto& vertices = mesh.getVertices();
    const Vertex& vertex = vertices[0];

    ASSERT_NEAR(vertex.uv_coord.u, 0.0, 1e-6);
    ASSERT_NEAR(vertex.uv_coord.v, 0.0, 1e-6);
}

TEST(SphereMeshBuilderTest, TestMeshFaceCount) {
    SphereMeshBuilder builder(1.0, 10, 10);
    Mesh mesh = builder.build();

    ASSERT_EQ(mesh.getFaces().size(), 200);  // 2 faces per segment * rings * segments
}

TEST(SphereMeshBuilderTest, BuildInvalidSize) {
    SphereMeshBuilder builder1(-1.0, 10, 10);
    Mesh mesh1 = builder1.build();

    EXPECT_EQ(mesh1.getVertices().size(), 0);
    EXPECT_EQ(mesh1.getFaces().size(), 0);

    SphereMeshBuilder builder2(1.0, -10, 10);
    Mesh mesh2 = builder2.build();

    EXPECT_EQ(mesh2.getVertices().size(), 0);
    EXPECT_EQ(mesh2.getFaces().size(), 0);

    SphereMeshBuilder builder3(1.0, 10, -10);
    Mesh mesh3 = builder3.build();

    EXPECT_EQ(mesh3.getVertices().size(), 0);
    EXPECT_EQ(mesh3.getFaces().size(), 0);
}