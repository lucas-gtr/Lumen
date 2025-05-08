#include <Eigen/Core>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "BVH/BVHBuilder.hpp"
#include "BVH/BVHNode.hpp"
#include "Geometry/Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) : m_vertices(vertices) {
  for(const auto& face : faces) {
    if(std::max({face.vertexIndices[0], face.vertexIndices[1], face.vertexIndices[2]}) <
       static_cast<int>(m_vertices.size())) {
      this->m_faces.push_back(face);
    }
  }
  computeTangentsAndBitangents();
}

void Mesh::computeTangentsAndBitangents() {
  for(const auto& face : m_faces) {
    const Vertex& v0 = m_vertices[face.vertexIndices[0]];
    const Vertex& v1 = m_vertices[face.vertexIndices[1]];
    const Vertex& v2 = m_vertices[face.vertexIndices[2]];

    const Eigen::Vector3d edge1 = v1.position - v0.position;
    const Eigen::Vector3d edge2 = v2.position - v0.position;

    const Eigen::Vector2d deltaUV1(v1.uvCoord.u - v0.uvCoord.u, v1.uvCoord.v - v0.uvCoord.v);
    const Eigen::Vector2d deltaUV2(v2.uvCoord.u - v0.uvCoord.u, v2.uvCoord.v - v0.uvCoord.v);

    const double f = 1.0 / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    const Eigen::Vector3d tangent   = f * (deltaUV2.y() * edge1 - deltaUV1.y() * edge2);
    const Eigen::Vector3d bitangent = f * (-deltaUV2.x() * edge1 + deltaUV1.x() * edge2);

    m_vertices[face.vertexIndices[0]].tangent += tangent;
    m_vertices[face.vertexIndices[1]].tangent += tangent;
    m_vertices[face.vertexIndices[2]].tangent += tangent;

    m_vertices[face.vertexIndices[0]].bitangent += bitangent;
    m_vertices[face.vertexIndices[1]].bitangent += bitangent;
    m_vertices[face.vertexIndices[2]].bitangent += bitangent;
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
    const auto& v0   = m_vertices[face.vertexIndices[0]].position;
    const auto& v1   = m_vertices[face.vertexIndices[1]].position;
    const auto& v2   = m_vertices[face.vertexIndices[2]].position;

    const Eigen::Vector3d min_bound = v0.cwiseMin(v1).cwiseMin(v2);
    const Eigen::Vector3d max_bound = v0.cwiseMax(v1).cwiseMax(v2);

    bvh_leaves.emplace_back(std::make_shared<BVHNode>(min_bound, max_bound, static_cast<int>(i)));
  }
  BVH::constructNode(m_bvh_root, bvh_leaves, 0, static_cast<int>(bvh_leaves.size()));
}