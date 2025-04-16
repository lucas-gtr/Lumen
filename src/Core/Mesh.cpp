#include "Core/Mesh.hpp"

#include <algorithm>
#include <vector>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) : vertices(vertices) {
  for(const auto& face : faces) {
    if(std::max({face.vertexIndices[0], face.vertexIndices[1], face.vertexIndices[2]}) <
       static_cast<int>(vertices.size())) {
      this->faces.push_back(face);
    }
  }
}