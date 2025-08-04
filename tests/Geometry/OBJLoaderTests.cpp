#include "Geometry/OBJLoader.hpp"
#include "Geometry/Mesh.hpp"
#include "Core/ImageTypes.hpp"

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

class OBJLoaderTest : public ::testing::Test {
protected:
  std::string testFilename;

  void SetUp() override {
    testFilename = "test_model.obj";

    std::ofstream file(testFilename);
    file << R"(# This is a comment
vr some random prefix
vt 0.1 0.2
vt 0.3 0.4

v 1.0 0.0 0.0
v 0.0 1.0 0.0
v 0.0 0.0 1.0
vn 0.0 0.0 1.0
vn 0.0 1.0 0.0
vn 1.0 0.0 0.0

# Face with full v/vt/vn format
f 1/1/1 2/2/2 3/1/3
)";
    file.close();
  }

  void TearDown() override {
    std::remove(testFilename.c_str());
  }
};

TEST_F(OBJLoaderTest, LoadOBJFileWithCommentsAndNoise) {
  Mesh mesh = OBJLoader::load(testFilename);

  ASSERT_EQ(mesh.getVertices().size(), 3);
  ASSERT_EQ(mesh.getFaces().size(), 1);

  const Vertex& v0 = mesh.getVertices()[0];
  const Vertex& v1 = mesh.getVertices()[1];
  const Vertex& v2 = mesh.getVertices()[2];

  EXPECT_EQ(v0.position, linalg::Vec3d(1.0, 0.0, 0.0));
  EXPECT_EQ(v1.position, linalg::Vec3d(0.0, 1.0, 0.0));
  EXPECT_EQ(v2.position, linalg::Vec3d(0.0, 0.0, 1.0));

  EXPECT_EQ(v0.normal, linalg::Vec3d(0.0, 0.0, 1.0));
  EXPECT_EQ(v1.normal, linalg::Vec3d(0.0, 1.0, 0.0));
  EXPECT_EQ(v2.normal, linalg::Vec3d(1.0, 0.0, 0.0));

  EXPECT_FLOAT_EQ(v0.uv_coord.u, 0.1f);
  EXPECT_FLOAT_EQ(v0.uv_coord.v, 0.2f);
  EXPECT_FLOAT_EQ(v1.uv_coord.u, 0.3f);
  EXPECT_FLOAT_EQ(v1.uv_coord.v, 0.4f);
  EXPECT_FLOAT_EQ(v2.uv_coord.u, 0.1f); 
  EXPECT_FLOAT_EQ(v2.uv_coord.v, 0.2f);

  const Face& face = mesh.getFaces()[0];
  EXPECT_EQ(face.vertex_indices[0], 0);
  EXPECT_EQ(face.vertex_indices[1], 1);
  EXPECT_EQ(face.vertex_indices[2], 2);
}

TEST(OBJLoaderEdgeCaseTest, LoadNonExistentFile) {
  const std::string nonexistentFile = "does_not_exist.obj";
  Mesh mesh = OBJLoader::load(nonexistentFile);

  EXPECT_TRUE(mesh.getVertices().empty());
  EXPECT_TRUE(mesh.getFaces().empty());
}
