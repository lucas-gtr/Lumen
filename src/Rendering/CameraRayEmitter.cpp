#include <linalg/Vec2.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Core/Ray.hpp"
#include "Rendering/CameraRayEmitter.hpp"

void CameraRayEmitter::initializeViewport(const RayEmitterParameters& parameters) {
  m_parameters = parameters;

  const double sensor_height = m_parameters.sensor_width / m_parameters.image_aspect_ratio;

  m_viewport_top_left_corner = generateCorner(-m_parameters.sensor_width * HALF, sensor_height * HALF);

  const linalg::Vec3d viewport_top_right_corner =
      generateCorner(m_parameters.sensor_width * HALF, sensor_height * HALF);
  const linalg::Vec3d viewport_bottom_left_corner =
      generateCorner(-m_parameters.sensor_width * HALF, -sensor_height * HALF);

  m_horizontal_vector = viewport_top_right_corner - m_viewport_top_left_corner;
  m_vertical_vector   = viewport_bottom_left_corner - m_viewport_top_left_corner;
}

linalg::Vec3d CameraRayEmitter::generateCorner(double x, double y) const {
  return m_parameters.camera_rotation_matrix * linalg::Vec3d(x, y, -m_parameters.focal_length) *
             m_parameters.focus_distance / m_parameters.focal_length +
         m_parameters.camera_position;
}

linalg::Vec3d CameraRayEmitter::getFocusPoint(double u, double v) const {
  return m_viewport_top_left_corner + u * m_horizontal_vector + v * m_vertical_vector;
}

linalg::Vec3d CameraRayEmitter::getRayOrigin() const {
  const linalg::Vec2d offset = randomPointInUnitDisk() * m_parameters.lens_radius;
  return linalg::Vec3d(offset.x, offset.y, 0.0) + m_parameters.camera_position;
}

Ray CameraRayEmitter::generateRay(double u, double v) const {
  const linalg::Vec3d origin      = getRayOrigin();
  const linalg::Vec3d focus_point = getFocusPoint(u, v);
  return Ray::FromPoint(origin, focus_point);
}