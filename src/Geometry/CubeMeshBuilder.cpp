#include <linalg/Vec3.hpp>
#include <vector>

#include "Core/CommonTypes.hpp"
#include "Core/MathConstants.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Geometry/Mesh.hpp"

CubeMeshBuilder::CubeMeshBuilder(double size) : m_size(size) {}

Mesh CubeMeshBuilder::build() const {
  if(m_size <= 0) {
    return {};
  }

  const double h = m_size * HALF;

  std::vector<Vertex> vertices(TOTAL_VERTICES);
  std::vector<Face>   faces;

  const std::vector<linalg::Vec3d> normals = {{0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}};

  const std::vector<TextureUV> uvs = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

  const std::vector<linalg::Vec3d> positions = {{-h, -h, h},  {h, -h, h},  {h, h, h},  {-h, h, h},
                                                {-h, -h, -h}, {h, -h, -h}, {h, h, -h}, {-h, h, -h}};

  const std::vector<std::vector<int>> face_indices = {
      {0, 1, 2, 3}, // front
      {5, 4, 7, 6}, // back
      {3, 2, 6, 7}, // top
      {4, 5, 1, 0}, // bottom
      {1, 5, 6, 2}, // right
      {4, 0, 3, 7}  // left
  };

  for(int i = 0; i < TOTAL_FACES; ++i) {
    const linalg::Vec3d& n = normals[i];
    for(int j = 0; j < VERTICES_PER_FACE; ++j) {
      Vertex v;
      v.position                          = positions[face_indices[i][j]];
      v.normal                            = n;
      v.uv_coord                          = uvs[j];
      vertices[i * VERTICES_PER_FACE + j] = v;
    }

    const int base = i * VERTICES_PER_FACE;
    faces.push_back({{base, base + 1, base + 2}});
    faces.push_back({{base, base + 2, base + 3}});
  }

  return {vertices, faces};
}
