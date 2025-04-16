#include "Rendering/CameraRayEmitter.hpp"
#include "gtest/gtest.h"
#include <Eigen/Geometry>

TEST(CameraRayEmitterTest, InitializeViewport) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = Eigen::Vector3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = Eigen::Matrix3d::Identity();
  parameters.lensRadius = 0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);
  Ray ray = emitter.generateRay(0.5, 0.5);
  
  EXPECT_TRUE(ray.origin.isApprox(Eigen::Vector3d(0.0, 0.0, 0.0), 1e-6));
  EXPECT_TRUE(ray.direction.isApprox(Eigen::Vector3d(0.0, 0.0, -1.0), 1e-6));
}

TEST(CameraRayEmitterTest, InitializeViewportRotation) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = Eigen::Vector3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = Eigen::AngleAxisd(M_PI / 2, Eigen::Vector3d::UnitY()).toRotationMatrix();
  parameters.lensRadius = 0;
  
  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);

  Ray ray = emitter.generateRay(0.5, 0.5);

  EXPECT_TRUE(ray.origin.isApprox(Eigen::Vector3d(0.0, 0.0, 0.0), 1e-6));
  EXPECT_TRUE(ray.direction.isApprox(Eigen::Vector3d(-1.0, 0.0, 0.0), 1e-6));
}

TEST(CameraRayEmitterTest, RayOriginWithLensRadius) {
  RayEmitterParameters parameters;
  parameters.sensorWidth = 36.0;
  parameters.imageAspectRatio = 1.5;
  parameters.focusDistance = 1.0;
  parameters.focalLength = 0.05;
  parameters.cameraPosition = Eigen::Vector3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = Eigen::Matrix3d::Identity();
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
  parameters.cameraPosition = Eigen::Vector3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = Eigen::Matrix3d::Identity();
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
  parameters.cameraPosition = Eigen::Vector3d(0.0, 0.0, 0.0);
  parameters.cameraRotationMatrix = Eigen::Matrix3d::Identity();
  parameters.lensRadius = 0.0;

  CameraRayEmitter emitter;
  emitter.initializeViewport(parameters);

  double u = 0.5, v = 0.5;
  Ray ray = emitter.generateRay(u, v);

  EXPECT_EQ(ray.origin, parameters.cameraPosition);
  EXPECT_TRUE(ray.direction.isApprox(Eigen::Vector3d(0.0, 0.0, -1.0), 1e-6));
}

