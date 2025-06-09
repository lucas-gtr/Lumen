#include "Rendering/CameraRayEmitter.hpp"

#include <gtest/gtest.h>

TEST(CameraRayEmitterTest, InitializeViewport) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = lin::Vec3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = lin::Mat3d::Identity();
  parameters.lensRadius = 0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);
  
  EXPECT_TRUE(ray.origin.isApprox(lin::Vec3d(0.0, 0.0, 0.0), 1e-6));
  EXPECT_TRUE(ray.direction.isApprox(lin::Vec3d(0.0, 0.0, -1.0), 1e-6));
}

TEST(CameraRayEmitterTest, InitializeViewportRotation) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = lin::Vec3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = lin::Mat3d({
    {  0.0,  0.0,  1.0 }, 
    {  0.0,  1.0,  0.0 }, 
    { -1.0,  0.0,  0.0 }});
  parameters.lensRadius = 0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);

  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_TRUE(ray.origin.isApprox(lin::Vec3d(0.0, 0.0, 0.0), 1e-6));
  EXPECT_TRUE(ray.direction.isApprox(lin::Vec3d(-1.0, 0.0, 0.0), 1e-6));
}

TEST(CameraRayEmitterTest, RayOriginWithLensRadius) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = lin::Vec3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = lin::Mat3d::Identity();
  parameters.lensRadius = 0.1;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_NE(ray.origin, parameters.cameraPosition);
}

TEST(CameraRayEmitterTest, RayOriginWithoutLensRadius) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = lin::Vec3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = lin::Mat3d::Identity();
  parameters.lensRadius = 0.0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_EQ(ray.origin, parameters.cameraPosition);
}

TEST(CameraRayEmitterTest, GenerateRay) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = lin::Vec3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = lin::Mat3d::Identity();
  parameters.lensRadius = 0.0;

  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);

  double u = 0.5, v = 0.5;
  Ray ray = emitter.generateRay(u, v);

  EXPECT_EQ(ray.origin, parameters.cameraPosition);
  EXPECT_TRUE(ray.direction.isApprox(lin::Vec3d(0.0, 0.0, -1.0), 1e-6));
}

