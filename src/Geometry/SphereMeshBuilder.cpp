#include <cmath>
#include <numbers>
#include <vector>

#include "Core/CommonTypes.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/SphereMeshBuilder.hpp"

SphereMeshBuilder::SphereMeshBuilder(double radius, int segments, int rings)
    : radius(radius), segments(segments), rings(rings) {}

Mesh SphereMeshBuilder::build() const {
  if(radius <= 0 || segments <= 0 || rings <= 0) {
    return {};
  }

  std::vector<Vertex> vertices;
  std::vector<Face>   faces;

  for(int i = 0; i <= rings; ++i) {
    const double theta    = i * std::numbers::pi / rings;
    const double sinTheta = sin(theta);
    const double cosTheta = cos(theta);

    for(int j = 0; j <= segments; ++j) {
      const double phi    = j * 2.0 * std::numbers::pi / segments;
      const double sinPhi = sin(phi);
      const double cosPhi = cos(phi);

      Vertex vertex;
      vertex.position.x = radius * sinTheta * cosPhi;
      vertex.position.y = radius * cosTheta;
      vertex.position.z = radius * sinTheta * sinPhi;

      vertex.normal.x = vertex.position.x / radius;
      vertex.normal.y = vertex.position.y / radius;
      vertex.normal.z = vertex.position.z / radius;

      vertex.uv_coord.u = static_cast<double>(j) / segments;
      vertex.uv_coord.v = static_cast<double>(i) / rings;

      vertices.push_back(vertex);
    }
  }

  for(int i = 0; i < rings; ++i) {
    for(int j = 0; j < segments; ++j) {
      int first  = (i * (segments + 1)) + j;
      int second = first + segments + 1;

      faces.push_back({{first + 1, second, first}});
      faces.push_back({{first + 1, second + 1, second}});
    }
  }

  return Mesh{vertices, faces};
}