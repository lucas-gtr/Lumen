#include <cmath>
#include <numbers>
#include <vector>

#include "Core/CommonTypes.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/SphereMeshBuilder.hpp"

SphereMeshBuilder::SphereMeshBuilder(double radius, int segments, int rings)
    : m_radius(radius), m_segments(segments), m_rings(rings) {}

Mesh SphereMeshBuilder::build() const {
  if(m_radius <= 0 || m_segments <= 0 || m_rings <= 0) {
    return {};
  }

  std::vector<Vertex> vertices;
  std::vector<Face>   faces;

  for(int i = 0; i <= m_rings; ++i) {
    const double theta     = i * std::numbers::pi / m_rings;
    const double sin_theta = sin(theta);
    const double cos_theta = cos(theta);

    for(int j = 0; j <= m_segments; ++j) {
      const double phi     = j * 2.0 * std::numbers::pi / m_segments;
      const double sin_phi = sin(phi);
      const double cos_phi = cos(phi);

      Vertex vertex;
      vertex.position.x = m_radius * sin_theta * cos_phi;
      vertex.position.y = m_radius * cos_theta;
      vertex.position.z = m_radius * sin_theta * sin_phi;

      vertex.normal.x = vertex.position.x / m_radius;
      vertex.normal.y = vertex.position.y / m_radius;
      vertex.normal.z = vertex.position.z / m_radius;

      vertex.uv_coord.u = static_cast<double>(j) / m_segments;
      vertex.uv_coord.v = static_cast<double>(i) / m_rings;

      vertices.push_back(vertex);
    }
  }

  for(int i = 0; i < m_rings; ++i) {
    for(int j = 0; j < m_segments; ++j) {
      int first  = (i * (m_segments + 1)) + j;
      int second = first + m_segments + 1;

      faces.push_back({{first + 1, second, first}});
      faces.push_back({{first + 1, second + 1, second}});
    }
  }

  return Mesh{vertices, faces};
}