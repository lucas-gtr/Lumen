/**
 * @file CameraRayEmitter.hpp
 * @brief Header file for the CameraRayEmitter class.
 */
#ifndef RENDERING_CAMERARAYEMITTER_HPP
#define RENDERING_CAMERARAYEMITTER_HPP

#include "Core/Math/Mat3.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Ray.hpp"

/**
 * @struct RayEmitterParameters
 * @brief Struct to hold parameters for ray emitter configuration.
 */
struct RayEmitterParameters {
  lin::Vec3d camera_position        = {0.0, 0.0, 0.0};
  lin::Mat3d camera_rotation_matrix = lin::Mat3d::Identity();
  double     sensor_width           = 0.0;
  double     focal_length           = 0.0;
  double     focus_distance         = 0.0;
  double     lens_radius            = 0.0;
  double     image_aspect_ratio     = 0.0;
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

  lin::Vec3d m_viewport_top_left_corner;
  lin::Vec3d m_horizontal_vector;
  lin::Vec3d m_vertical_vector;

  lin::Vec3d generateCorner(double x, double y) const;
  lin::Vec3d getRayOrigin() const;
  lin::Vec3d getFocusPoint(double u, double v) const;

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