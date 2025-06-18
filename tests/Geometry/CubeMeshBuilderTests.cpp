#include "Geometry/CubeMeshBuilder.hpp"

#include <gtest/gtest.h>

TEST(CubeMeshBuilderTest, BuildCorrectSize) {
  CubeMeshBuilder builder(2.0);
  Mesh mesh = builder.build();

  EXPECT_EQ(mesh.getVertices().size(), 24);
  EXPECT_EQ(mesh.getFaces().size(), 12);
}

TEST(CubeMeshBuilderTest, CorrectVertexPositions) {
  CubeMeshBuilder builder(2.0);
  Mesh mesh = builder.build();
  const auto& vertices = mesh.getVertices();

  EXPECT_EQ(vertices[0].position, lin::Vec3d(-1.0, -1.0, 1.0));
  EXPECT_EQ(vertices[1].position, lin::Vec3d(1.0, -1.0, 1.0));
  EXPECT_EQ(vertices[2].position, lin::Vec3d(1.0, 1.0, 1.0));
  EXPECT_EQ(vertices[3].position, lin::Vec3d(-1.0, 1.0, 1.0));
  
  EXPECT_EQ(vertices[4].position, lin::Vec3d(1.0, -1.0, -1.0));
  EXPECT_EQ(vertices[5].position, lin::Vec3d(-1.0, -1.0, -1.0));
  EXPECT_EQ(vertices[6].position, lin::Vec3d(-1.0, 1.0, -1.0));
  EXPECT_EQ(vertices[7].position, lin::Vec3d(1.0, 1.0, -1.0));
}

TEST(CubeMeshBuilderTest, CorrectNormals) {
  CubeMeshBuilder builder(2.0);
  Mesh mesh = builder.build();
  const auto& vertices = mesh.getVertices();

  EXPECT_EQ(vertices[0].normal, lin::Vec3d(0.0, 0.0, 1.0));
  EXPECT_EQ(vertices[1].normal, lin::Vec3d(0.0, 0.0, 1.0));
  EXPECT_EQ(vertices[2].normal, lin::Vec3d(0.0, 0.0, 1.0));
  EXPECT_EQ(vertices[3].normal, lin::Vec3d(0.0, 0.0, 1.0));

  EXPECT_EQ(vertices[4].normal, lin::Vec3d(0.0, 0.0, -1.0));
  EXPECT_EQ(vertices[5].normal, lin::Vec3d(0.0, 0.0, -1.0));
  EXPECT_EQ(vertices[6].normal, lin::Vec3d(0.0, 0.0, -1.0));
  EXPECT_EQ(vertices[7].normal, lin::Vec3d(0.0, 0.0, -1.0));
}

TEST(CubeMeshBuilderTest, CorrectUVCoordinates) {
  CubeMeshBuilder builder(2.0);
  Mesh mesh = builder.build();
  const auto& vertices = mesh.getVertices();

  EXPECT_EQ(vertices[0].uv_coord.u, 0.0);
  EXPECT_EQ(vertices[0].uv_coord.v, 0.0);
  EXPECT_EQ(vertices[1].uv_coord.u, 1.0);
  EXPECT_EQ(vertices[1].uv_coord.v, 0.0);
  EXPECT_EQ(vertices[2].uv_coord.u, 1.0);
  EXPECT_EQ(vertices[2].uv_coord.v, 1.0);
  EXPECT_EQ(vertices[3].uv_coord.u, 0.0);
  EXPECT_EQ(vertices[3].uv_coord.v, 1.0);
}

TEST(CubeMeshBuilderTest, BuildWithNegativeSize) {
  CubeMeshBuilder builder(-2.0);
  Mesh mesh = builder.build();

  EXPECT_EQ(mesh.getVertices().size(), 0);
  EXPECT_EQ(mesh.getFaces().size(), 0);
}
