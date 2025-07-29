#include "Geometry/Mesh.hpp"

#include <gtest/gtest.h>

TEST(MeshTest, DefaultConstructorTest) {
    Mesh mesh;

    EXPECT_EQ(mesh.getVertices().size(), 0);
    EXPECT_EQ(mesh.getFaces().size(), 0);
}

TEST(MeshTest, ParameterizedConstructorTest) {
    Vertex v1 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
    Vertex v2 {linalg::Vec3d(4.0, 5.0, 6.0), linalg::Vec3d(0.0, 0.0, 1.0), {0.25, 0.25}};
    Vertex v3 {linalg::Vec3d(7.0, 8.0, 9.0), linalg::Vec3d(1.0, 0.0, 0.0), {0.75, 0.75}};
    Face f1 {{0, 1, 2}};

    std::vector<Vertex> vertices = {v1, v2, v3};
    std::vector<Face> faces = {f1};

    Mesh mesh(vertices, faces);

    EXPECT_EQ(mesh.getVertices().size(), 3);
    EXPECT_EQ(mesh.getFaces().size(), 1);
}

TEST(MeshTest, InvalidConstructorTest) {
  Vertex v1 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
  Vertex v2 {linalg::Vec3d(4.0, 5.0, 6.0), linalg::Vec3d(0.0, 0.0, 1.0), {0.25, 0.25}};
  Vertex v3 {linalg::Vec3d(7.0, 8.0, 9.0), linalg::Vec3d(1.0, 0.0, 0.0), {0.75, 0.75}};
  Face f1 {{0, 1, 2}};
  Face f2 {{1, 2, 3}}; // Invalid face index

  std::vector<Vertex> vertices = {v1, v2, v3};
  std::vector<Face> faces = {f1, f2};

  Mesh mesh(vertices, faces);

  EXPECT_EQ(mesh.getVertices().size(), 3);
  EXPECT_EQ(mesh.getFaces().size(), 1); // Invalid face should not be added
}

TEST(MeshTest, VertexSize) {
    EXPECT_EQ(Vertex::ValuePerVertex(), 14); // position (3) + normal (3) + uv_coord (2) + tangent (3) + bitangent (3)
}

TEST(MeshTest, GetVertexTest) {
    Vertex v1 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
    Vertex v2 {linalg::Vec3d(4.0, 5.0, 6.0), linalg::Vec3d(0.0, 0.0, 1.0), {0.25, 0.25}};
    Vertex v3 {linalg::Vec3d(7.0, 8.0, 9.0), linalg::Vec3d(1.0, 0.0, 0.0), {0.75, 0.75}};
    Face f1 {{0, 1, 2}};

    std::vector<Vertex> vertices = {v1, v2, v3};
    std::vector<Face> faces = {f1};

    Mesh mesh(vertices, faces);

    EXPECT_EQ(mesh.getVertex(0), v1);
    EXPECT_EQ(mesh.getVertex(1), v2);
}

TEST(MeshTest, MeshEquality) {
  Vertex v1 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
  Vertex v2 {linalg::Vec3d(4.0, 5.0, 6.0), linalg::Vec3d(0.0, 0.0, 1.0), {0.25, 0.25}};
  Vertex v3 {linalg::Vec3d(7.0, 8.0, 9.0), linalg::Vec3d(1.0, 0.0, 0.0), {0.75, 0.75}};

  std::vector<Vertex> vertices1 = {v1, v2, v3};
  std::vector<Vertex> vertices2 = {v3, v2, v1};
  std::vector<Vertex> vertices3 = {v1, v2, v3};
  std::vector<Face> faces1 = {{0, 1, 2}};
  std::vector<Face> faces2 = {{1, 2, 0}};
  std::vector<Face> faces3 = {{0, 1, 2}};

  Mesh mesh1(vertices1, faces1);
  Mesh mesh2(vertices1, faces2);
  EXPECT_NE(mesh1, mesh2); // Different faces but same vertices

  Mesh mesh3(vertices1, faces1);
  Mesh mesh4(vertices3, faces3);
  EXPECT_EQ(mesh3, mesh4); // Same vertices and same faces

  Mesh mesh5(vertices1, faces1);
  Mesh mesh6(vertices2, faces3);
  EXPECT_NE(mesh5, mesh6); // Different vertices but same faces

  Mesh mesh7(vertices1, faces2);
  Mesh mesh8(vertices2, faces3);
  EXPECT_NE(mesh7, mesh8); // Different vertices and different faces
}

TEST(MeshTest, VertexEquality) {
  Vertex v1 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
  Vertex v2 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.25, 0.25}};
  EXPECT_NE(v1, v2); // Identical positions and normals, different texcoords

  Vertex v3 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 0.0, 1.0), {0.5, 0.5}};
  EXPECT_NE(v1, v3); // Identical positions and texcoords, different normals

  Vertex v4 {linalg::Vec3d(4.0, 5.0, 6.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
  EXPECT_NE(v1, v4); // Identical normals and texcoords, different positions

  Vertex v5 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
  EXPECT_EQ(v1, v5); // Identical positions, normals, and texcoords

  Vertex v6 {linalg::Vec3d(7.0, 8.0, 9.0), linalg::Vec3d(1.0, 0.0, 0.0), {0.75, 0.75}};
  EXPECT_NE(v1, v6); // Different positions, normals, and texcoords
}

TEST(MeshTest, FaceEquality) {
    Face f1 {{0, 1, 2}};
    Face f2 {{2, 1, 0}}; 
    EXPECT_NE(f1, f2); // Different order of indices

    Face f3 {{1, 1, 2}};
    EXPECT_NE(f1, f3); // Different first index

    Face f4 {{0, 2, 2}};
    EXPECT_NE(f1, f4); // Different second index

    Face f5 {{0, 1, 3}};
    EXPECT_NE(f1, f5); // Different third index

    Face f6 {{0, 1, 2}};
    EXPECT_EQ(f1, f6);  // Identical indices
}

TEST(MeshTest, BuildBVH) {
    Vertex v1 {linalg::Vec3d(1.0, 2.0, 3.0), linalg::Vec3d(0.0, 1.0, 0.0), {0.5, 0.5}};
    Vertex v2 {linalg::Vec3d(4.0, 5.0, 6.0), linalg::Vec3d(0.0, 0.0, 1.0), {0.25, 0.25}};
    Vertex v3 {linalg::Vec3d(7.0, 8.0, 9.0), linalg::Vec3d(1.0, 0.0, 0.0), {0.75, 0.75}};
    Face f1 {{0, 1, 2}};

    std::vector<Vertex> vertices = {v1, v2, v3};
    std::vector<Face> faces = {f1};

    Mesh mesh(vertices, faces);
    mesh.buildBVH();

    EXPECT_EQ(mesh.getBVHRoot(), nullptr);
    // EXPECT_EQ(mesh.getBVHRoot()->getLeafIndex(), 0);
}
