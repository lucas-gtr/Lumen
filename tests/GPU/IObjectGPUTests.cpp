#include <gtest/gtest.h>
#include "GPU/IObjectGPU.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Geometry/Mesh.hpp"
#include <linalg/Mat3.hpp>
#include <linalg/Mat4.hpp>

class TestObjectGPU : public IObjectGPU {
public:
    explicit TestObjectGPU(Object3D* object) : IObjectGPU(object) {}
    void uploadToGPU() override { }
    void release() override {}

    using IObjectGPU::vertices;
    using IObjectGPU::indices;
    using IObjectGPU::dataSize;
    using IObjectGPU::indicesSize;
};

class IObjectGPUTest : public ::testing::Test {
protected:
    Object3D object;

    void SetUp() override {
        // 1 triangle
        Vertex v1, v2, v3;
        v1.position = {1.0, 0.0, 0.0};
        v2.position = {0.0, 1.0, 0.0};
        v3.position = {0.0, 0.0, 1.0};

        Face face;
        face.vertex_indices[0] = 0;
        face.vertex_indices[1] = 1;
        face.vertex_indices[2] = 2;

        std::vector<Vertex> vertices = {v1, v2, v3};
        std::vector<Face> faces = {face};
        Mesh mesh(vertices, faces);

        object.setMesh(mesh);
    }
};

TEST_F(IObjectGPUTest, InitializationTest) {
    object.setPosition({1.0, 2.0, 3.0});
    object.setRotationDeg({45.0, 90.0, 180.0});

    TestObjectGPU gpuObject(&object);

    EXPECT_EQ(gpuObject.getIndexCount(), 3u);

    const float* modelMatrix = gpuObject.getModelMatrix();
    const float* normalMatrix = gpuObject.getNormalMatrix();

    linalg::Mat4f expectedModel = linalg::Mat4f(object.getTransformationMatrix());
    linalg::Mat3f expectedNormal = linalg::Mat3f(object.getNormalMatrix());

    for (int i = 0; i < 16; ++i)
        EXPECT_FLOAT_EQ(modelMatrix[i], expectedModel.data()[i]);

    for (int i = 0; i < 9; ++i)
        EXPECT_FLOAT_EQ(normalMatrix[i], expectedNormal.data()[i]);
}

TEST_F(IObjectGPUTest, VertexAndIndexDataTest) {
    TestObjectGPU gpuObject(&object);

    const std::vector<float>& vertices = gpuObject.vertices();
    const std::vector<unsigned int>& indices = gpuObject.indices();

    EXPECT_EQ(indices.size(), 3u);
    EXPECT_EQ(vertices.size(), 3u * Vertex::ValuePerVertex());

    EXPECT_EQ(indices[0], 0u);
    EXPECT_EQ(indices[1], 1u);
    EXPECT_EQ(indices[2], 2u);

    EXPECT_EQ(gpuObject.dataSize(), 3u * Vertex::ValuePerVertex() * sizeof(float));
    EXPECT_EQ(gpuObject.indicesSize(), 3u * sizeof(unsigned int));

    EXPECT_FLOAT_EQ(vertices[0], 1.0f); // x
    EXPECT_FLOAT_EQ(vertices[1], 0.0f); // y
    EXPECT_FLOAT_EQ(vertices[2], 0.0f); // z
}
