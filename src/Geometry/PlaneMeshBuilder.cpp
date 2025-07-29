#include <linalg/Vec3.hpp>
#include <vector>

#include "Core/MathConstants.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/PlaneMeshBuilder.hpp"

PlaneMeshBuilder::PlaneMeshBuilder(double width, double length) : m_width(width), m_length(length) {}

Mesh PlaneMeshBuilder::build() const {
  if(m_width <= 0 || m_length <= 0) {
    return {};
  }

  std::vector<Vertex> vertices(4);
  std::vector<Face>   faces;

  vertices[0].position = linalg::Vec3d(-m_width * HALF, 0, -m_length * HALF);
  vertices[1].position = linalg::Vec3d(m_width * HALF, 0, -m_length * HALF);
  vertices[2].position = linalg::Vec3d(m_width * HALF, 0, m_length * HALF);
  vertices[3].position = linalg::Vec3d(-m_width * HALF, 0, m_length * HALF);

  for(auto& vertex : vertices) {
    vertex.normal   = linalg::Vec3d(0.0, 1.0, 0.0);
    vertex.uv_coord = {vertex.position.x + m_width * HALF, vertex.position.z + m_length * HALF};
  }

  faces.push_back({{0, 2, 1}});
  faces.push_back({{0, 3, 2}});

  return {vertices, faces};
}