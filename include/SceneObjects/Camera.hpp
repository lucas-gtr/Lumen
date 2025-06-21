/**
 * @file Camera.hpp
 * @brief Header file for the Camera class.
 */
#ifndef SCENEOBJECTS_CAMERA_HPP
#define SCENEOBJECTS_CAMERA_HPP

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Observer.hpp"
#include "Core/Transform.hpp"

#include <algorithm>

/**
 * @class Camera
 * @brief Class representing a camera with adjustable parameters such as field of view, focal length, aperture, and
 * shutter speed. Inherits from the Transform class.
 */
class Camera : public Transform {
private:
  double m_horizontal_fov = DEFAULT_HORIZONTAL_FOV; // in degrees

  double m_focal_length  = DEFAULT_FOCAL_LENGTH;  // in mm
  double m_aperture      = DEFAULT_APERTURE;      // f-stop
  double m_shutter_speed = DEFAULT_SHUTTER_SPEED; // in seconds

  double m_focus_distance = DEFAULT_FOCUS_DISTANCE; // in meters

  double m_sensor_width; // in mm
  double m_lens_radius;  // in mm

  double m_near_plane = DEFAULT_CAMERA_NEAR_PLANE; // in meters
  double m_far_plane  = DEFAULT_CAMERA_FAR_PLANE;  // in meters

  double getUpdatedLensRadius() const;
  double getUpdatedSensorWidth() const;
  double getUpdatedFov() const;

  Observer<> m_camera_changed_observer;

public:
  Camera(); ///< Default constructor.

  Camera(const Camera&)            = delete;
  Camera& operator=(const Camera&) = delete;
  Camera(Camera&&)                 = delete;
  Camera& operator=(Camera&&)      = delete;

  /**
   * @brief Gets the observer for camera changes.
   * @return A reference to the observer that notifies when camera parameters change.
   */
  Observer<>& getCameraChangedObserver() { return m_camera_changed_observer; }

  /**
   * @brief Gets the minimum sensor width in millimeters.
   * @return The minimum sensor width in millimeters.
   */
  double getMinSensorWidth() const;

  /**
   * @brief Gets the maximum sensor width in millimeters.
   * @return The maximum sensor width in millimeters.
   */
  double getMaxSensorWidth() const;

  /**
   * @brief Gets the current field of view in degrees.
   * @return The current field of view in degrees.
   */
  double getFovX() const { return m_horizontal_fov; }

  /**
   * @brief Sets the horizontal field of view.
   * @param fov The horizontal field of view in degrees.
   */
  void setHorizontalFov(double fov);

  /**
   * @brief Gets the current sensor width in millimeters.
   * @return The current sensor width in millimeters.
   */
  double getSensorWidth() const { return m_sensor_width * MM_TO_M; }

  /**
   * @brief Sets the sensor width.
   * @param sensor_width The sensor width in millimeters.
   */
  void setSensorWidth(double sensor_width);

  /**
   * @brief Gets the current focal length in meters.
   * @return The current focal length in meters.
   */
  double getFocalLength() const { return m_focal_length * MM_TO_M; }

  /**
   * @brief Sets the focal length.
   * @param focal_length The focal length in millimeters.
   */
  void setFocalLength(double focal_length);

  /**
   * @brief Gets the current aperture (f-stop).
   * @return The current aperture.
   */
  double getAperture() const { return m_aperture; }

  /**
   * @brief Sets the aperture (f-stop).
   * @param aperture The aperture (f-stop).
   */
  void setAperture(double aperture);

  /**
   * @brief Gets the current shutter speed in seconds.
   * @return The current shutter speed in seconds.
   */
  double getShutterSpeed() const { return m_shutter_speed; }

  /**
   * @brief Sets the shutter speed.
   * @param shutterSpeed The shutter speed in seconds.
   */
  void setShutterSpeed(double shutterSpeed);

  /**
   * @brief Gets the current focus distance in meters.
   * @return The current focus distance in meters.
   */
  double getFocusDistance() const { return m_focus_distance; }

  /**
   * @brief Sets the focus distance.
   * @param focus_distance The focus distance in meters.
   */
  void setFocusDistance(double focus_distance);
  /**
   * @brief Gets the lens radius in millimeters.
   * @return The lens radius in millimeters.
   */
  double getLensRadius() const { return m_lens_radius * MM_TO_M; }

  /**
   * @brief Gets the near clipping plane distance in meters.
   * @return The near clipping plane distance in meters.
   */
  double getNearPlane() const { return m_near_plane; }

  /**
   * @brief Sets the near clipping plane distance.
   * @param nearPlane The near clipping plane distance in meters.
   */
  void setNearPlane(double nearPlane);

  /**
   * @brief Gets the far clipping plane distance in meters.
   * @return The far clipping plane distance in meters.
   */
  double getFarPlane() const { return m_far_plane; }

  /**
   * @brief Sets the far clipping plane distance.
   * @param farPlane The far clipping plane distance in meters.
   */
  void setFarPlane(double farPlane);

  ~Camera() = default; ///< Default destructor.
};

#endif // SCENEOBJECTS_CAMERA_HPP