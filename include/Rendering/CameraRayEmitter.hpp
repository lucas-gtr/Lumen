/**
 * @file CameraRayEmitter.hpp
 * @brief Header file for the CameraRayEmitter class.
 */
#ifndef RENDERING_CAMERARAYEMITTER_HPP
#define RENDERING_CAMERARAYEMITTER_HPP

#include "Core/Ray.hpp"

#include <Eigen/Core>

/**
 * @struct RayEmitterParameters
 * @brief Struct to hold parameters for ray emitter configuration.
 */
struct RayEmitterParameters {
  Eigen::Vector3d cameraPosition;
  Eigen::Matrix3d cameraRotationMatrix;
  double          sensorWidth;
  double          focalLength;
  double          focusDistance;
  double          lensRadius;
  double          imageAspectRatio;
};

/**
 * @class CameraRayEmitter
 * @brief Class to generate rays from the camera.
 *
 * This class is responsible for generating rays for a camera based on a given set of parameters,
 * including position, rotation, focal length, and sensor characteristics.
 */
class CameraRayEmitter {
private:
  RayEmitterParameters m_parameters;

  Eigen::Vector3d m_viewportTopLeftCorner;
  Eigen::Vector3d m_horizontalVector;
  Eigen::Vector3d m_verticalVector;

  Eigen::Vector3d generateCorner(double x, double y) const;
  Eigen::Vector3d getRayOrigin() const;
  Eigen::Vector3d getFocusPoint(double u, double v) const;

public:
  CameraRayEmitter() = default; ///< Default constructor.

  CameraRayEmitter(const CameraRayEmitter&)            = delete;
  CameraRayEmitter& operator=(const CameraRayEmitter&) = delete;
  CameraRayEmitter(CameraRayEmitter&&)                 = delete;
  CameraRayEmitter& operator=(CameraRayEmitter&&)      = delete;

  /**
   * @brief Initialize the ray emitter with camera parameters.
   * @param parameters The parameters to initialize the ray emitter.
   */
  void initializeViewport(const RayEmitterParameters& parameters);

  /**
   * @brief Generate a ray from the camera based on normalized screen coordinates.
   * @param u Normalized horizontal coordinate (0 to 1).
   * @param v Normalized vertical coordinate (0 to 1).
   * @return The generated ray.
   */
  Ray generateRay(double u, double v) const;

  ~CameraRayEmitter() = default; ///< Default destructor.
};

#endif // RENDERING_CAMERARAYEMITTER_HPP