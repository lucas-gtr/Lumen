#include <algorithm>
#include <cstddef>
#include <linalg/Vec2.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>
#include <memory>
#include <vector>

#include "BVH/BVHBuilder.hpp"
#include "BVH/BVHNode.hpp"
#include "Core/CommonTypes.hpp"
#include "Geometry/Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) : m_vertices(vertices) {
  for(const auto& face : faces) {
    if(std::max({face.vertex_indices[0], face.vertex_indices[1], face.vertex_indices[2]}) <
       static_cast<int>(m_vertices.size())) {
      this->m_faces.push_back(face);
    }
  }
  computeTangentsAndBitangents();
}

void Mesh::computeTangentsAndBitangents() {
  for(const auto& face : m_faces) {
    const Vertex& v0 = m_vertices[face.vertex_indices[0]];
    const Vertex& v1 = m_vertices[face.vertex_indices[1]];
    const Vertex& v2 = m_vertices[face.vertex_indices[2]];

    const linalg::Vec3d edge1 = v1.position - v0.position;
    const linalg::Vec3d edge2 = v2.position - v0.position;

    const linalg::Vec2d delta_u_v1 = {v1.uv_coord.u - v0.uv_coord.u, v1.uv_coord.v - v0.uv_coord.v};
    const linalg::Vec2d delta_u_v2 = {v2.uv_coord.u - v0.uv_coord.u, v2.uv_coord.v - v0.uv_coord.v};

    const double f = 1.0 / (delta_u_v1.x * delta_u_v2.y - delta_u_v2.x * delta_u_v1.y);

    const linalg::Vec3d tangent   = f * (delta_u_v2.y * edge1 - delta_u_v1.y * edge2);
    const linalg::Vec3d bitangent = f * (-delta_u_v2.x * edge1 + delta_u_v1.x * edge2);

    m_vertices[face.vertex_indices[0]].tangent += tangent;
    m_vertices[face.vertex_indices[1]].tangent += tangent;
    m_vertices[face.vertex_indices[2]].tangent += tangent;

    m_vertices[face.vertex_indices[0]].bitangent += bitangent;
    m_vertices[face.vertex_indices[1]].bitangent += bitangent;
    m_vertices[face.vertex_indices[2]].bitangent += bitangent;
  }

  for(auto& vertex : m_vertices) {
    vertex.tangent.normalize();
    vertex.bitangent.normalize();
  }
}

void Mesh::buildBVH() {
  m_bvh_root = std::make_shared<BVHNode>();

  std::vector<std::shared_ptr<BVHNode>> bvh_leaves;
  bvh_leaves.reserve(m_faces.size());

  for(size_t i = 0; i < m_faces.size(); ++i) {
    const auto& face = m_faces[i];
    const auto& v0   = m_vertices[face.vertex_indices[0]].position;
    const auto& v1   = m_vertices[face.vertex_indices[1]].position;
    const auto& v2   = m_vertices[face.vertex_indices[2]].position;

    const linalg::Vec3d min_bound = linalg::cwiseMin(v0, linalg::cwiseMin(v1, v2));
    const linalg::Vec3d max_bound = linalg::cwiseMax(v0, linalg::cwiseMax(v1, v2));

    bvh_leaves.emplace_back(std::make_shared<BVHNode>(min_bound, max_bound, static_cast<int>(i)));
  }
  BVH::constructNode(m_bvh_root, bvh_leaves, 0, static_cast<int>(bvh_leaves.size()));
}