#include "Rendering/CameraRayEmitter.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/lin.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"

void CameraRayEmitter::initializeViewport(const RayEmitterParameters& parameters) {
  m_parameters = parameters;

  const double sensor_height = m_parameters.sensor_width / m_parameters.image_aspect_ratio;

  m_viewport_top_left_corner = generateCorner(-m_parameters.sensor_width * HALF, sensor_height * HALF);

  const lin::Vec3d viewportTopRightCorner   = generateCorner(m_parameters.sensor_width * HALF, sensor_height * HALF);
  const lin::Vec3d viewportBottomLeftCorner = generateCorner(-m_parameters.sensor_width * HALF, -sensor_height * HALF);

  m_horizontal_vector = viewportTopRightCorner - m_viewport_top_left_corner;
  m_vertical_vector   = viewportBottomLeftCorner - m_viewport_top_left_corner;
}

lin::Vec3d CameraRayEmitter::generateCorner(double x, double y) const {
  return m_parameters.camera_rotation_matrix * lin::Vec3d(x, y, -m_parameters.focal_length) *
             m_parameters.focus_distance / m_parameters.focal_length +
         m_parameters.camera_position;
}

lin::Vec3d CameraRayEmitter::getFocusPoint(double u, double v) const {
  return m_viewport_top_left_corner + u * m_horizontal_vector + v * m_vertical_vector;
}

lin::Vec3d CameraRayEmitter::getRayOrigin() const {
  const lin::Vec2d offset = randomPointInUnitDisk() * m_parameters.lens_radius;
  return lin::Vec3d(offset.x, offset.y, 0.0) + m_parameters.camera_position;
}

Ray CameraRayEmitter::generateRay(double u, double v) const {
  const lin::Vec3d origin      = getRayOrigin();
  const lin::Vec3d focus_point = getFocusPoint(u, v);
  return Ray::FromPoint(origin, focus_point);
}