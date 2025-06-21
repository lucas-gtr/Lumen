#include <algorithm>
#include <cmath>

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "SceneObjects/Camera.hpp"

Camera::Camera() : m_sensor_width(getUpdatedSensorWidth()), m_lens_radius(getUpdatedLensRadius()) {}

double Camera::getUpdatedLensRadius() const { return (m_focal_length / m_aperture) * HALF; }

double Camera::getUpdatedSensorWidth() const { return 2 * m_focal_length * tan(m_horizontal_fov * HALF * DEG_TO_RAD); }

double Camera::getUpdatedFov() const { return 2 * atan(m_sensor_width / (2 * m_focal_length)) * RAD_TO_DEG; }

double Camera::getMinSensorWidth() const { return 2 * m_focal_length * tan((MIN_HORIZONTAL_FOV * HALF) * DEG_TO_RAD); }

double Camera::getMaxSensorWidth() const { return 2 * m_focal_length * tan((MAX_HORIZONTAL_FOV * HALF) * DEG_TO_RAD); }

void Camera::setHorizontalFov(double fov) {
  m_horizontal_fov = std::clamp(fov, MIN_HORIZONTAL_FOV, MAX_HORIZONTAL_FOV);
  m_sensor_width   = getUpdatedSensorWidth();
  m_camera_changed_observer.notify();
}

void Camera::setSensorWidth(double sensor_width) {
  m_sensor_width   = std::clamp(sensor_width, getMinSensorWidth(), getMaxSensorWidth());
  m_horizontal_fov = getUpdatedFov();
  m_camera_changed_observer.notify();
}

void Camera::setFocalLength(double focal_length) {
  m_focal_length   = std::clamp(focal_length, MIN_FOCAL_LENGTH, MAX_FOCAL_LENGTH);
  m_horizontal_fov = getUpdatedFov();
  m_lens_radius    = getUpdatedLensRadius();
  m_camera_changed_observer.notify();
}

void Camera::setAperture(double aperture) {
  m_aperture    = std::clamp(aperture, MIN_APERTURE, MAX_APERTURE);
  m_lens_radius = getUpdatedLensRadius();
  m_camera_changed_observer.notify();
}

void Camera::setShutterSpeed(double shutterSpeed) {
  m_shutter_speed = std::clamp(shutterSpeed, MIN_SHUTTER_SPEED, MAX_SHUTTER_SPEED);
  m_camera_changed_observer.notify();
}

void Camera::setFocusDistance(double focus_distance) {
  m_focus_distance = std::clamp(focus_distance, MIN_FOCUS_DISTANCE, MAX_FOCUS_DISTANCE);
  m_camera_changed_observer.notify();
}

void Camera::setNearPlane(double nearPlane) {
  m_near_plane = std::clamp(nearPlane, MIN_CAMERA_PLANE, m_far_plane - MIN_CAMERA_PLANE);
  m_camera_changed_observer.notify();
}

void Camera::setFarPlane(double farPlane) {
  m_far_plane = std::clamp(farPlane, m_near_plane + MIN_CAMERA_PLANE, MAX_CAMERA_PLANE);
  m_camera_changed_observer.notify();
}
