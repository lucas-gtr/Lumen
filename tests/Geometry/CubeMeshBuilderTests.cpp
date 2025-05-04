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

  EXPECT_EQ(vertices[0].position, Eigen::Vector3d(-1.0, -1.0, 1.0));
  EXPECT_EQ(vertices[1].position, Eigen::Vector3d(1.0, -1.0, 1.0));
  EXPECT_EQ(vertices[2].position, Eigen::Vector3d(1.0, 1.0, 1.0));
  EXPECT_EQ(vertices[3].position, Eigen::Vector3d(-1.0, 1.0, 1.0));
  
  EXPECT_EQ(vertices[4].position, Eigen::Vector3d(1.0, -1.0, -1.0));
  EXPECT_EQ(vertices[5].position, Eigen::Vector3d(-1.0, -1.0, -1.0));
  EXPECT_EQ(vertices[6].position, Eigen::Vector3d(-1.0, 1.0, -1.0));
  EXPECT_EQ(vertices[7].position, Eigen::Vector3d(1.0, 1.0, -1.0));
}

TEST(CubeMeshBuilderTest, CorrectNormals) {
  CubeMeshBuilder builder(2.0);
  Mesh mesh = builder.build();
  const auto& vertices = mesh.getVertices();

  EXPECT_EQ(vertices[0].normal, Eigen::Vector3d(0.0, 0.0, 1.0));
  EXPECT_EQ(vertices[1].normal, Eigen::Vector3d(0.0, 0.0, 1.0));
  EXPECT_EQ(vertices[2].normal, Eigen::Vector3d(0.0, 0.0, 1.0));
  EXPECT_EQ(vertices[3].normal, Eigen::Vector3d(0.0, 0.0, 1.0));

  EXPECT_EQ(vertices[4].normal, Eigen::Vector3d(0.0, 0.0, -1.0));
  EXPECT_EQ(vertices[5].normal, Eigen::Vector3d(0.0, 0.0, -1.0));
  EXPECT_EQ(vertices[6].normal, Eigen::Vector3d(0.0, 0.0, -1.0));
  EXPECT_EQ(vertices[7].normal, Eigen::Vector3d(0.0, 0.0, -1.0));
}

TEST(CubeMeshBuilderTest, CorrectUVCoordinates) {
  CubeMeshBuilder builder(2.0);
  Mesh mesh = builder.build();
  const auto& vertices = mesh.getVertices();

  EXPECT_EQ(vertices[0].uvCoord.u, 0.0);
  EXPECT_EQ(vertices[0].uvCoord.v, 0.0);
  EXPECT_EQ(vertices[1].uvCoord.u, 1.0);
  EXPECT_EQ(vertices[1].uvCoord.v, 0.0);
  EXPECT_EQ(vertices[2].uvCoord.u, 1.0);
  EXPECT_EQ(vertices[2].uvCoord.v, 1.0);
  EXPECT_EQ(vertices[3].uvCoord.u, 0.0);
  EXPECT_EQ(vertices[3].uvCoord.v, 1.0);
}

TEST(CubeMeshBuilderTest, BuildWithNegativeSize) {
  CubeMeshBuilder builder(-2.0);
  Mesh mesh = builder.build();

  EXPECT_EQ(mesh.getVertices().size(), 0);
  EXPECT_EQ(mesh.getFaces().size(), 0);
}
