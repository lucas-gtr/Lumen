#include <gtest/gtest.h>
#include "Rendering/PathTracer/RayIntersection.hpp"
#include "Scene/Scene.hpp"
#include "Geometry/CubeMeshBuilder.hpp"

#include <limits>

static constexpr double EPSILON = 1e-6;

TEST(RayIntersectionTest, ValidIntersection) {
    Ray ray = Ray::FromPoint({0, 0, -1}, {0, 0, 1});
    linalg::Vec3d p0(0, 1, 0), p1(-1, -1, 0), p2(1, -1, 0);

    double distance = std::numeric_limits<double>::max();
    linalg::Vec3d bary_coords;

    bool success = RayIntersection::getTriangleIntersection(ray, p0, p1, p2, distance, bary_coords);
    EXPECT_TRUE(success);
    EXPECT_NEAR(distance, 1.0, EPSILON);
    EXPECT_TRUE(bary_coords.isApprox(linalg::Vec3d(0.5, 0.25, 0.25), EPSILON));
}

TEST(RayIntersectionTest, RayParallelToTriangle) {
    Ray ray = Ray::FromPoint({0, 0, 0}, {1, 0, 0});
    linalg::Vec3d p0(0, 1, 1), p1(-1, -1, 1), p2(1, -1, 1);

    double distance = std::numeric_limits<double>::max();
    linalg::Vec3d bary_coords;

    bool success = RayIntersection::getTriangleIntersection(ray, p0, p1, p2, distance, bary_coords);
    EXPECT_FALSE(success);
}

TEST(RayIntersectionTest, IntersectionOutsideTriangle) {
    Ray ray = Ray::FromPoint({0, 0, -1}, {2, 0, 1});
    linalg::Vec3d p0(0, 1, 0), p1(-1, -1, 0), p2(1, -1, 0);

    double distance = std::numeric_limits<double>::max();
    linalg::Vec3d bary_coords;

    bool success = RayIntersection::getTriangleIntersection(ray, p0, p1, p2, distance, bary_coords);
    EXPECT_FALSE(success);
}

TEST(RayIntersectionTest, IntersectionBehindRayOrigin) {
    Ray ray = Ray::FromPoint({0, 0, 1}, {0, 0, 2});
    linalg::Vec3d p0(0, 1, 0), p1(-1, -1, 0), p2(1, -1, 0);

    double distance = std::numeric_limits<double>::max();
    linalg::Vec3d bary_coords;

    bool success = RayIntersection::getTriangleIntersection(ray, p0, p1, p2, distance, bary_coords);
    EXPECT_FALSE(success);
}

TEST(RayIntersectionTest, IntersectionOnEdge) {
    Ray ray = Ray::FromPoint({0, 0, 0}, {0, 0, -1});
    linalg::Vec3d p0(1, 0, -1), p1(-1, 0, -1), p2(0, 1, -1);

    double distance = std::numeric_limits<double>::max();
    linalg::Vec3d bary_coords;

    bool success = RayIntersection::getTriangleIntersection(ray, p0, p1, p2, distance, bary_coords);
    EXPECT_TRUE(success);
    EXPECT_NEAR(distance, 1.0, EPSILON);
    EXPECT_EQ(bary_coords, linalg::Vec3d(0.5, 0.5, 0));
}

TEST(RayIntersectionTest, IntersectionOnVertex) {
    Ray ray = Ray::FromPoint({0, 0, 0}, {0, 0, -1});
    linalg::Vec3d p0(0, 0, -1), p1(0, -1, -1), p2(1, -1, -1);

    double distance = std::numeric_limits<double>::max();
    linalg::Vec3d bary_coords;

    bool success = RayIntersection::getTriangleIntersection(ray, p0, p1, p2, distance, bary_coords);
    EXPECT_TRUE(success);
    EXPECT_NEAR(distance, 1.0, EPSILON);
    EXPECT_EQ(bary_coords, linalg::Vec3d(1.0, 0.0, 0.0));
}

TEST(RayIntersectionTest, MeshIntersectionSingleTriangleHit) {
    Vertex v0{{0, 1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{-1, -1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{1, -1, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray = Ray::FromPoint({0, 0, -1}, {0, 0, 1});

    RayHitInfo hit = RayIntersection::getMeshIntersection(ray, mesh);

    EXPECT_NEAR(hit.distance, 1.0, EPSILON);
}

TEST(RayIntersectionTest, MeshIntersectionNoHit) {
    Vertex v0{{10, 10, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{11, 10, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{10, 11, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray = Ray::FromPoint({0, 0, -1}, {0, 0, 1});

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
    Face faceB{{3, 4, 5}};;  //(z = 5)

    std::vector<Vertex> vertices = {v0a, v1a, v2a, v0b, v1b, v2b};
    std::vector<Face> faces = {faceA, faceB};
    Mesh mesh(vertices, faces);

    Ray ray = Ray::FromPoint({0, 0, 0}, {0, 0, 1});

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

    Ray ray = Ray::FromPoint({0, 0, 0}, {0, 0, 1});

    RayHitInfo hit = RayIntersection::getMeshIntersection(ray, mesh);

    EXPECT_NEAR(hit.distance, 2.0, EPSILON);
}

TEST(RayIntersectionTest, RayIntersectionObject) {
    Vertex v0{{0, 1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{-1, -1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{1, -1, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray = Ray::FromPoint({0, -1.1, -1}, {0, -1.1, 0});

    std::unique_ptr<Object3D> object = std::make_unique<Object3D>(mesh);

    auto material = Material();
    object->setMaterial(&material);

    RayHitInfo hit = RayIntersection::getObjectIntersection(ray, object.get());

    EXPECT_EQ(hit.distance, std::numeric_limits<double>::max());

    object->setScale(1.2);
    
    
    hit = RayIntersection::getObjectIntersection(ray, object.get());
    EXPECT_NEAR(hit.distance, 1.0, EPSILON);
}

TEST(RayIntersectionTest, RayIntersectionScene) {
    Vertex v0{{0, 1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{-1, -1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{1, -1, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray = Ray::FromPoint({0, -1.1, -1}, {0, -1.1, 0});

    std::unique_ptr<Object3D> object = std::make_unique<Object3D>(mesh);

    auto material = Material();
    object->setMaterial(&material);

    Scene scene;
    scene.addObject("1", std::move(object));

    RayHitInfo hit = RayIntersection::getSceneIntersection(ray, &scene);
    EXPECT_EQ(hit.distance, std::numeric_limits<double>::max());

    scene.getObjectList()[0]->setScale(1.2);
    hit = RayIntersection::getSceneIntersection(ray, &scene);
    EXPECT_NEAR(hit.distance, 1.0, EPSILON);
}

TEST(RayIntersectionTest, RayIntersectionAABB) {
    Ray ray = Ray::FromPoint({0, 0, 1}, {0, 0, -1});
    linalg::Vec3d min_bound(-0.5, -0.5, -0.5);
    linalg::Vec3d max_bound(0.5, 0.5, 0.5);

    double hit_distance = std::numeric_limits<double>::max();
    const linalg::Vec3d inv_dir = ray.direction.cwiseInverse();
    bool hit = RayIntersection::getAABBIntersection(ray.origin, inv_dir, min_bound, max_bound, hit_distance);
    EXPECT_TRUE(hit);
    EXPECT_NEAR(hit_distance, 0.5, EPSILON);
}

TEST(RayIntersectionTest, RayIntersectionAABBNoHit) {
    Ray ray = Ray::FromPoint({0, 0, 1}, {0, 0, 2});
    linalg::Vec3d min_bound(-0.5, -0.5, -0.5);
    linalg::Vec3d max_bound(0.5, 0.5, 0.5);

    double hit_distance = std::numeric_limits<double>::max();
    const linalg::Vec3d inv_dir = ray.direction.cwiseInverse();
    bool hit = RayIntersection::getAABBIntersection(ray.origin, inv_dir, min_bound, max_bound, hit_distance);
    EXPECT_FALSE(hit);
}

TEST(RayIntersectionTest, RayIntersectionBVH) {
    Ray ray = Ray::FromPoint({0, 0, 2}, {0, 0, 0});
    linalg::Vec3d min_bound(-1.0, -1.0, -1.0);
    linalg::Vec3d max_bound(1.0, 1.0, 1.0);

    std::shared_ptr<BVHNode> root = std::make_shared<BVHNode>(min_bound, max_bound);

    root->getLeftChild() = std::make_shared<BVHNode>(linalg::Vec3d(-1.0, -1.0, -1.0),
                                                     linalg::Vec3d(0.0, 0.0, 0.0), 0);
    root->getRightChild() = std::make_shared<BVHNode>(linalg::Vec3d(0.0, 0.0, 0.0),
                                                      linalg::Vec3d(1.0, 1.0, 1.0), 1);
    std::vector<RayIntersection::RayBVHHitInfo> hits;

    hits = RayIntersection::getBVHIntersection(ray, root.get());
    EXPECT_EQ(hits.size(), 2);
    EXPECT_EQ(hits[0].index_to_check, 1);
    EXPECT_EQ(hits[1].index_to_check, 0);
}

TEST(RayIntersectionTest, RayIntersectionBVHNoHit) {
    Ray ray = Ray::FromPoint({0, 0, 2}, {0, 0, 3});
    linalg::Vec3d min_bound(-1.0, -1.0, -1.0);
    linalg::Vec3d max_bound(1.0, 1.0, 1.0);

    std::shared_ptr<BVHNode> root = std::make_shared<BVHNode>(min_bound, max_bound);

    root->getLeftChild() = std::make_shared<BVHNode>(linalg::Vec3d(-1.0, -1.0, -1.0),
                                                     linalg::Vec3d(0.0, 0.0, 0.0), 0);
    root->getRightChild() = std::make_shared<BVHNode>(linalg::Vec3d(2.0, 2.0, 2.0),
                                                      linalg::Vec3d(3.0, 3.0, 3.0), 1);
    std::vector<RayIntersection::RayBVHHitInfo> hits;

    hits = RayIntersection::getBVHIntersection(ray, root.get());
    EXPECT_EQ(hits.size(), 0);
}

TEST(RayIntersectionTest, RayIntersectionSceneWithBVHEarlyExit) {
    Ray ray = Ray::FromPoint({0, 0, 2}, {0, 0, 0});
    Mesh mesh1 = CubeMeshBuilder(1.0).build();
    std::unique_ptr<Object3D> object1 = std::make_unique<Object3D>(mesh1);
    object1->setPosition(linalg::Vec3d(0.0, 0.0, -2.0));

    Mesh mesh2 = CubeMeshBuilder(1.0).build();
    std::unique_ptr<Object3D> object2 = std::make_unique<Object3D>(mesh2);
    
    auto material = Material();
    object1->setMaterial(&material);
    object2->setMaterial(&material);

    Scene scene;
    scene.addObject("1", std::move(object1));
    scene.addObject("2", std::move(object2));
    scene.buildBVH();

    RayHitInfo hit = RayIntersection::getSceneIntersection(ray, &scene);
    EXPECT_EQ(hit.distance, 1.5);
}

TEST(RayIntersectionTest, UpdateNormalWithTangentSpace) {
    linalg::Vec3d normal(0, 0, 1);
    linalg::Vec3d tangent(1, 0, 0);
    linalg::Vec3d bitangent(0, 1, 0);

    RayHitInfo hit;
    hit.normal = normal;
    hit.tangent = tangent;
    hit.bitangent = bitangent;

    std::unique_ptr<Texture> normal_texture = std::make_unique<Texture>();
    normal_texture->setValue(ColorRGB(0.75, 1.0, 0.5));
    std::unique_ptr<Material> material = std::make_unique<Material>();
    material->setNormalTexture(normal_texture.get());
    hit.material = material.get();

    RayIntersection::updateNormalWithTangentSpace(hit);

    EXPECT_TRUE(hit.normal.isApprox(linalg::Vec3d(0.5, 1, 0).normalized(), EPSILON));
}

TEST(RayIntersectionTest, GetObjectNameFromHit) {
    Vertex v0{{0, 1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v1{{-1, -1, 0}, {0, 0, 1}, {0, 0}};
    Vertex v2{{1, -1, 0}, {0, 0, 1}, {0, 0}};
    Face face{{0, 1, 2}};

    Mesh mesh({v0, v1, v2}, {face});
    Ray ray_1 = Ray::FromPoint({0, 0, 3}, {0, 0, 0});
    Ray ray_2 = Ray::FromPoint({0, 0, -3}, {0, 0, 0});

    std::unique_ptr<Object3D> object_1 = std::make_unique<Object3D>(mesh);
    object_1->setPosition(linalg::Vec3d(0, 0, 0));

    std::unique_ptr<Object3D> object_2 = std::make_unique<Object3D>(mesh);
    object_2->setPosition(linalg::Vec3d(0, 0, -1));

    Scene scene;
    scene.addObject("TestObject1", std::move(object_1));
    scene.addObject("TestObject2", std::move(object_2));

    std::string object_name_1 = RayIntersection::getObjectNameFromHit(ray_1, &scene);
    std::string object_name_2 = RayIntersection::getObjectNameFromHit(ray_2, &scene);

    EXPECT_EQ(object_name_1, "TestObject1");
    EXPECT_EQ(object_name_2, "TestObject2");
}
