#include <Eigen/Core>
#include <vector>

#include "Core/MathConstants.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/PlaneMeshBuilder.hpp"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
PlaneMeshBuilder::PlaneMeshBuilder(double width, double length) : width(width), length(length) {}

Mesh PlaneMeshBuilder::build() const {
  if(width <= 0 || length <= 0) {
    return {};
  }

  std::vector<Vertex> vertices(4);
  std::vector<Face>   faces;

  vertices[0].position = Eigen::Vector3d(-width * HALF, 0, -length * HALF);
  vertices[1].position = Eigen::Vector3d(width * HALF, 0, -length * HALF);
  vertices[2].position = Eigen::Vector3d(width * HALF, 0, length * HALF);
  vertices[3].position = Eigen::Vector3d(-width * HALF, 0, length * HALF);

  for(auto& vertex : vertices) {
    vertex.normal  = Eigen::Vector3d(0.0, 1.0, 0.0);
    vertex.uvCoord = {vertex.position.x() / width + HALF, vertex.position.z() / length + HALF};
  }

  faces.push_back({{0, 1, 2}});
  faces.push_back({{0, 2, 3}});

  return {vertices, faces};
}