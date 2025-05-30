#include "Rendering/CameraRayEmitter.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"

void CameraRayEmitter::initializeViewport(const RayEmitterParameters& parameters) {
  m_parameters = parameters;

  const double sensor_height = m_parameters.sensorWidth / m_parameters.imageAspectRatio;

  m_viewportTopLeftCorner = generateCorner(-m_parameters.sensorWidth * HALF, sensor_height * HALF);

  const lin::Vec3 viewportTopRightCorner   = generateCorner(m_parameters.sensorWidth * HALF, sensor_height * HALF);
  const lin::Vec3 viewportBottomLeftCorner = generateCorner(-m_parameters.sensorWidth * HALF, -sensor_height * HALF);

  m_horizontalVector = viewportTopRightCorner - m_viewportTopLeftCorner;
  m_verticalVector   = viewportBottomLeftCorner - m_viewportTopLeftCorner;
}

lin::Vec3 CameraRayEmitter::generateCorner(double x, double y) const {
  return m_parameters.cameraRotationMatrix * lin::Vec3(x, y, -m_parameters.focalLength) * m_parameters.focusDistance /
             m_parameters.focalLength +
         m_parameters.cameraPosition;
}

lin::Vec3 CameraRayEmitter::getFocusPoint(double u, double v) const {
  return m_viewportTopLeftCorner + u * m_horizontalVector + v * m_verticalVector;
}

lin::Vec3 CameraRayEmitter::getRayOrigin() const {
  const lin::Vec2 offset = randomPointInUnitDisk() * m_parameters.lensRadius;
  return lin::Vec3(offset.x, offset.y, 0.0) + m_parameters.cameraPosition;
}

Ray CameraRayEmitter::generateRay(double u, double v) const {
  const lin::Vec3 origin      = getRayOrigin();
  const lin::Vec3 focus_point = getFocusPoint(u, v);
  return Ray::FromPoint(origin, focus_point);
}