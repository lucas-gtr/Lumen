#include "Rendering/CameraRayEmitter.hpp"
#include "Core/Ray.hpp"

#include <gtest/gtest.h>

TEST(CameraRayEmitterTest, InitializeViewport) {
  RayEmitterParameters parameters;
  parameters.sensor_width = 36.0;
  parameters.image_aspect_ratio = 1.5;
  parameters.focus_distance = 1.0;
  parameters.focal_length = 0.05;
  parameters.camera_position = linalg::Vec3d(0.0, 0.0, 0.0);
  parameters.camera_rotation_matrix = linalg::Mat3d::Identity();
  parameters.lens_radius = 0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);
  
  EXPECT_TRUE(ray.origin.isApprox(linalg::Vec3d(0.0, 0.0, 0.0), 1e-6));
  EXPECT_TRUE(ray.direction.isApprox(linalg::Vec3d(0.0, 0.0, -1.0), 1e-6));
}

TEST(CameraRayEmitterTest, InitializeViewportRotation) {
  RayEmitterParameters parameters;
  parameters.sensor_width = 36.0;
  parameters.image_aspect_ratio = 1.5;
  parameters.focus_distance = 1.0;
  parameters.focal_length = 0.05;
  parameters.camera_position = linalg::Vec3d(0.0, 0.0, 0.0);
  parameters.camera_rotation_matrix = linalg::Mat3d({
    {  0.0,  0.0,  1.0 }, 
    {  0.0,  1.0,  0.0 }, 
    { -1.0,  0.0,  0.0 }});
  parameters.lens_radius = 0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);

  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_TRUE(ray.origin.isApprox(linalg::Vec3d(0.0, 0.0, 0.0), 1e-6));
  EXPECT_TRUE(ray.direction.isApprox(linalg::Vec3d(-1.0, 0.0, 0.0), 1e-6));
}

TEST(CameraRayEmitterTest, RayOriginWithLensRadius) {
  RayEmitterParameters parameters;
  parameters.sensor_width = 36.0;
  parameters.image_aspect_ratio = 1.5;
  parameters.focus_distance = 1.0;
  parameters.focal_length = 0.05;
  parameters.camera_position = linalg::Vec3d(0.0, 0.0, 0.0);
  parameters.camera_rotation_matrix = linalg::Mat3d::Identity();
  parameters.lens_radius = 0.1;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_NE(ray.origin, parameters.camera_position);
}

TEST(CameraRayEmitterTest, RayOriginWithoutLensRadius) {
  RayEmitterParameters parameters;
  parameters.sensor_width = 36.0;
  parameters.image_aspect_ratio = 1.5;
  parameters.focus_distance = 1.0;
  parameters.focal_length = 0.05;
  parameters.camera_position = linalg::Vec3d(0.0, 0.0, 0.0);
  parameters.camera_rotation_matrix = linalg::Mat3d::Identity();
  parameters.lens_radius = 0.0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_EQ(ray.origin, parameters.camera_position);
}

TEST(CameraRayEmitterTest, GenerateRay) {
  RayEmitterParameters parameters;
  parameters.sensor_width = 36.0;
  parameters.image_aspect_ratio = 1.5;
  parameters.focus_distance = 1.0;
  parameters.focal_length = 0.05;
  parameters.camera_position = linalg::Vec3d(0.0, 0.0, 0.0);
  parameters.camera_rotation_matrix = linalg::Mat3d::Identity();
  parameters.lens_radius = 0.0;

  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);

  double u = 0.5, v = 0.5;
  Ray ray = emitter.generateRay(u, v);

  EXPECT_EQ(ray.origin, parameters.camera_position);
  EXPECT_TRUE(ray.direction.isApprox(linalg::Vec3d(0.0, 0.0, -1.0), 1e-6));
}

