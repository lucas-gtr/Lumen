#include "Geometry/Mesh.hpp"

#include <Eigen/Core>
#include <algorithm>
#include <vector>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) : vertices(vertices) {
  for(const auto& face : faces) {
    if(std::max({face.vertexIndices[0], face.vertexIndices[1], face.vertexIndices[2]}) <
       static_cast<int>(vertices.size())) {
      this->faces.push_back(face);
    }
  }
  computeTangentsAndBitangents();
}

void Mesh::computeTangentsAndBitangents() {
  for(const auto& face : faces) {
    const Vertex& v0 = vertices[face.vertexIndices[0]];
    const Vertex& v1 = vertices[face.vertexIndices[1]];
    const Vertex& v2 = vertices[face.vertexIndices[2]];

    const Eigen::Vector3d edge1 = v1.position - v0.position;
    const Eigen::Vector3d edge2 = v2.position - v0.position;

    const Eigen::Vector2d deltaUV1(v1.uvCoord.u - v0.uvCoord.u, v1.uvCoord.v - v0.uvCoord.v);
    const Eigen::Vector2d deltaUV2(v2.uvCoord.u - v0.uvCoord.u, v2.uvCoord.v - v0.uvCoord.v);

    const double f = 1.0 / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    const Eigen::Vector3d tangent   = f * (deltaUV2.y() * edge1 - deltaUV1.y() * edge2);
    const Eigen::Vector3d bitangent = f * (-deltaUV2.x() * edge1 + deltaUV1.x() * edge2);

    vertices[face.vertexIndices[0]].tangent += tangent;
    vertices[face.vertexIndices[1]].tangent += tangent;
    vertices[face.vertexIndices[2]].tangent += tangent;

    vertices[face.vertexIndices[0]].bitangent += bitangent;
    vertices[face.vertexIndices[1]].bitangent += bitangent;
    vertices[face.vertexIndices[2]].bitangent += bitangent;
  }

  for(auto& vertex : vertices) {
    vertex.tangent.normalize();
    vertex.bitangent.normalize();
  }
}