/**
 * @file Mesh.hpp
 * @brief Header file for the Mesh class, the Vertex and Face structures.
 */
#ifndef GEOMETRY_MESH_HPP
#define GEOMETRY_MESH_HPP

#include <array>
#include <cstddef>
#include <linalg/Vec3.hpp>
#include <memory>
#include <vector>

#include "Core/ImageTypes.hpp"

class BVHNode;

/**
 * @struct Vertex
 * @brief Structure representing a vertex in a 3D mesh.
 */
struct Vertex {
  linalg::Vec3d position  = {0.0, 0.0, 0.0}; ///< Position of the vertex in 3D space.
  linalg::Vec3d normal    = {0.0, 0.0, 0.0}; ///< Normal vector at the vertex.
  TextureUV     uv_coord  = {0, 0};          ///< Texture coordinates of the vertex.
  linalg::Vec3d tangent   = {0.0, 0.0, 0.0}; ///< Tangent vector at the vertex.
  linalg::Vec3d bitangent = {0.0, 0.0, 0.0}; ///< Bitangent vector at the vertex.

  /**
   * @brief Equality operator for comparing two vertices.
   * @param other The vertex to compare with.
   * @return True if the vertices are equal, false otherwise.
   */
  bool operator==(const Vertex& other) const {
    return position == other.position && normal == other.normal && uv_coord.u == other.uv_coord.u &&
           uv_coord.v == other.uv_coord.v;
  }

  /**
   * @brief Returns the size of a vertex in terms of the number of values it contains.
   * @return The number of values in a vertex.
   */
  static constexpr size_t ValuePerVertex() {
    return 3 + 3 + 2 + 3 + 3; // position (3), normal (3), uv_coord (2), tangent (3), bitangent (3)
  }
};

/**
 * @struct Face
 * @brief Structure representing a face in a 3D mesh, defined by three vertices.
 */
struct Face {
  std::array<int, 3> vertex_indices = {0, 0, 0}; ///< Indices of the vertices making up the face.

  /**
   * @brief Equality operator for comparing two faces.
   * @param other The face to compare with.
   * @return True if the faces are equal, false otherwise.
   */
  bool operator==(const Face& other) const {
    return vertex_indices[0] == other.vertex_indices[0] && vertex_indices[1] == other.vertex_indices[1] &&
           vertex_indices[2] == other.vertex_indices[2];
  }
};

/**
 * @class Mesh
 * @brief Class representing a 3D mesh, containing vertices and faces.
 */
class Mesh {
private:
  std::vector<Vertex> m_vertices;
  std::vector<Face>   m_faces;

  std::shared_ptr<BVHNode> m_bvh_root;

  void computeTangentsAndBitangents();

public:
  Mesh() = default; ///< Default constructor.

  /**
   * @brief Constructor to initialize a Mesh with a set of vertices and faces.
   * @param vertices A vector containing the vertices of the mesh.
   * @param faces A vector containing the faces of the mesh.
   */
  Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces);

  Mesh(const Mesh&)            = default; ///< Default copy constructor.
  Mesh& operator=(const Mesh&) = default; ///< Default copy assignment operator.
  Mesh(Mesh&&)                 = default; ///< Default move constructor.
  Mesh& operator=(Mesh&&)      = default; ///< Default move assignment operator.

  /**
   * @brief Retrieves the list of vertices in the mesh.
   * @return A const reference to the list of vertices.
   */
  const std::vector<Vertex>& getVertices() const { return m_vertices; }

  /**
   * @brief Retrieves a specific vertex by index.
   * @param index The index of the vertex.
   * @return A const reference to the vertex at the given index.
   */
  const Vertex& getVertex(int index) const { return m_vertices[index]; }

  /**
   * @brief Retrieves the list of faces in the mesh.
   * @return A const reference to the list of faces.
   */
  const std::vector<Face>& getFaces() const { return m_faces; }

  /**
   * @brief Equality operator for comparing two meshes.
   * @param other The mesh to compare with.
   * @return True if the meshes are equal, false otherwise.
   */
  bool operator==(const Mesh& other) const { return m_vertices == other.m_vertices && m_faces == other.m_faces; }

  /**
   * @brief Builds the bounding volume hierarchy (BVH) for the mesh.
   */
  void buildBVH();

  /**
   * @brief Retrieves the bounding volume hierarchy (BVH) root node of the mesh.
   * @return A shared pointer to the BVH root node.
   */
  const BVHNode* getBVHRoot() const { return m_bvh_root.get(); }

  ~Mesh() = default; ///< Default destructor.
};

#endif // GEOMETRY_MESH_HPP
