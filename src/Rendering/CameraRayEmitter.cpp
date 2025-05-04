#include <Eigen/Core>

#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"

void CameraRayEmitter::initializeViewport(const RayEmitterParameters& parameters) {
  m_parameters = parameters;

  const double sensor_height = m_parameters.sensorWidth / m_parameters.imageAspectRatio;

  m_viewportTopLeftCorner = generateCorner(-m_parameters.sensorWidth * HALF, sensor_height * HALF);

  const Eigen::Vector3d viewportTopRightCorner = generateCorner(m_parameters.sensorWidth * HALF, sensor_height * HALF);
  const Eigen::Vector3d viewportBottomLeftCorner =
      generateCorner(-m_parameters.sensorWidth * HALF, -sensor_height * HALF);

  m_horizontalVector = viewportTopRightCorner - m_viewportTopLeftCorner;
  m_verticalVector   = viewportBottomLeftCorner - m_viewportTopLeftCorner;
}

Eigen::Vector3d CameraRayEmitter::generateCorner(double x, double y) const {
  return m_parameters.cameraRotationMatrix * Eigen::Vector3d(x, y, -m_parameters.focalLength) *
             m_parameters.focusDistance / m_parameters.focalLength +
         m_parameters.cameraPosition;
}

Eigen::Vector3d CameraRayEmitter::getFocusPoint(double u, double v) const {
  return m_viewportTopLeftCorner + u * m_horizontalVector + v * m_verticalVector;
}

Eigen::Vector3d CameraRayEmitter::getRayOrigin() const {
  const Eigen::Vector2d offset = randomPointInUnitDisk() * m_parameters.lensRadius;
  return Eigen::Vector3d(offset[0], offset[1], 0.0) + m_parameters.cameraPosition;
}

Ray CameraRayEmitter::generateRay(double u, double v) const {
  const Eigen::Vector3d origin      = getRayOrigin();
  const Eigen::Vector3d focus_point = getFocusPoint(u, v);
  return {origin, focus_point};
}