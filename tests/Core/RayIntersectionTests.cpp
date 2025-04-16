#include <gtest/gtest.h>
#include "Core/RayIntersection.hpp"
#include <limits>

constexpr double EPSILON = 1e-8;

TEST(RayIntersectionTest, ValidIntersection) {
    Ray ray{{0, 0, -1}, {0, 0, 1}};
    Eigen::Vector3d p0(0, 1, 0), p1(-1, -1, 0), p2(1, -1, 0);

    double distance = RayIntersection::getTriangleIntersection(ray, p0, p1, p2);
    EXPECT_NEAR(distance, 1.0, EPSILON);
}

TEST(RayIntersectionTest, RayParallelToTriangle) {
    Ray ray{{0, 0, 0}, {1, 0, 0}};
    Eigen::Vector3d p0(0, 1, 1), p1(-1, -1, 1), p2(1, -1, 1);

    double distance = RayIntersection::getTriangleIntersection(ray, p0, p1, p2);
    EXPECT_LT(distance, 0.0);
}

TEST(RayIntersectionTest, IntersectionOutsideTriangle) {
    Ray ray{{0, 0, -1}, {2, 0, 1}};
    Eigen::Vector3d p0(0, 1, 0), p1(-1, -1, 0), p2(1, -1, 0);

    double distance = RayIntersection::getTriangleIntersection(ray, p0, p1, p2);
    EXPECT_LT(distance, 0.0);
}

TEST(RayIntersectionTest, IntersectionBehindRayOrigin) {
    Ray ray{{0, 0, 1}, {0, 0, 2}};
    Eigen::Vector3d p0(0, 1, 0), p1(-1, -1, 0), p2(1, -1, 0);

    double distance = RayIntersection::getTriangleIntersection(ray, p0, p1, p2);
    EXPECT_LT(distance, 0.0);
}

TEST(RayIntersectionTest, IntersectionOnEdge) {
    Ray ray{{0, 0, 0}, {0, 0, -1}};
    Eigen::Vector3d p0(1, 0, -1), p1(-1, 0, -1), p2(0, 1, -1);

    double distance = RayIntersection::getTriangleIntersection(ray, p0, p1, p2);
    EXPECT_NEAR(distance, 1.0, EPSILON);
}

TEST(RayIntersectionTest, IntersectionOnVertex) {
    Ray ray{{0, 0, 0}, {0, 0, -1}};
    Eigen::Vector3d p0(0, 0, -1), p1(0, -1, -1), p2(1, -1, -1);

    double distance = RayIntersection::getTriangleIntersection(ray, p0, p1, p2);
    EXPECT_NEAR(distance, 1.0, EPSILON);
}

TEST(RayIntersectionTest, MeshIntersectionSingleTriangleHit) {
    Vertex v0{{0, 1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{-1, -1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{1, -1, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray{{0, 0, -1}, {0, 0, 1}};

    RayHitInfo hit = RayIntersection::getMeshIntersection(ray, mesh);

    EXPECT_NEAR(hit.distance, 1.0, EPSILON);
    EXPECT_EQ(hit.color, Eigen::Vector3d(1.0, 1.0, 1.0));
}

TEST(RayIntersectionTest, MeshIntersectionNoHit) {
    Vertex v0{{10, 10, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{11, 10, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{10, 11, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray{{0, 0, -1}, {0, 0, 1}};

    RayHitInfo hit = RayIntersection::getMeshIntersection(ray, mesh);

    EXPECT_EQ(hit.distance, std::numeric_limits<double>::max());
}

TEST(RayIntersectionTest, TwoTrianglesBehindEachOtherNearestIsReturned) {
    Vertex v0a{{-1, -1, 2}, {0, 0, -1}, {0, 0}};
    Vertex v1a{{1, -1, 2}, {0, 0, -1}, {0, 0}};
    Vertex v2a{{0, 1, 2}, {0, 0, -1}, {0, 0}};
    Face faceA{{0, 1, 2}};  // (z = 2)

    Vertex v0b{{-1, -1, 5}, {0, 0, -1}, {0, 0}};
    Vertex v1b{{1, -1, 5}, {0, 0, -1}, {0, 0}};
    Vertex v2b{{0, 1, 5}, {0, 0, -1}, {0, 0}};
    Face faceB{{3, 4, 5}};  //(z = 5)

    std::vector<Vertex> vertices = {v0a, v1a, v2a, v0b, v1b, v2b};
    std::vector<Face> faces = {faceA, faceB};
    Mesh mesh(vertices, faces);

    Ray ray{{0, 0, 0}, {0, 0, 1}};

    RayHitInfo hit = RayIntersection::getMeshIntersection(ray, mesh);

    EXPECT_NEAR(hit.distance, 2.0, EPSILON);
}

TEST(RayIntersectionTest, ReverseOrderTrianglesBehindEachOtherNearestIsStillReturned) {
    Vertex v0a{{-1, -1, 5}, {0, 0, -1}, {0, 0}};
    Vertex v1a{{1, -1, 5}, {0, 0, -1}, {0, 0}};
    Vertex v2a{{0, 1, 5}, {0, 0, -1}, {0, 0}};
    Face faceA{{0, 1, 2}};  // (z = 5)

    Vertex v0b{{-1, -1, 2}, {0, 0, -1}, {0, 0}};
    Vertex v1b{{1, -1, 2}, {0, 0, -1}, {0, 0}};
    Vertex v2b{{0, 1, 2}, {0, 0, -1}, {0, 0}};
    Face faceB{{3, 4, 5}};  // (z = 2)

    std::vector<Vertex> vertices = {v0a, v1a, v2a, v0b, v1b, v2b};
    std::vector<Face> faces = {faceA, faceB};
    Mesh mesh(vertices, faces);

    Ray ray{{0, 0, 0}, {0, 0, 1}};

    RayHitInfo hit = RayIntersection::getMeshIntersection(ray, mesh);

    EXPECT_NEAR(hit.distance, 2.0, EPSILON);
}

TEST(RayIntersectionTest, RayIntersectionObject) {
    Vertex v0{{0, 1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{-1, -1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{1, -1, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray{{0, -1.1, -1}, {0, -1.1, 0}};

    std::unique_ptr<Object3D> object = std::make_unique<Object3D>(mesh);
    RayHitInfo hit = RayIntersection::getObjectIntersection(ray, object.get());

    EXPECT_EQ(hit.distance, std::numeric_limits<double>::max());

    object->setScale(1.2);
    hit = RayIntersection::getObjectIntersection(ray, object.get());
    EXPECT_NEAR(hit.distance, 1.0, EPSILON);
}
