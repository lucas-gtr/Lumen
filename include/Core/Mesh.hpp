/**
 * @file Mesh.hpp
 * @brief Header file for the Mesh class, the Vertex and Face structures.
 */
#ifndef CORE_MESH_HPP
#define CORE_MESH_HPP

#include <Eigen/Core>
#include <array>
#include <vector>

/**
 * @struct Vertex
 * @brief Structure representing a vertex in a 3D mesh.
 */
struct Vertex {
  Eigen::Vector3d position; ///< Position of the vertex in 3D space.
  Eigen::Vector3d normal;   ///< Normal vector at the vertex.
  Eigen::Vector2d texCoord; ///< Texture coordinates of the vertex.

  /**
   * @brief Equality operator for comparing two vertices.
   * @param other The vertex to compare with.
   * @return True if the vertices are equal, false otherwise.
   */
  bool operator==(const Vertex& other) const {
    return position == other.position && normal == other.normal && texCoord == other.texCoord;
  }
};

/**
 * @struct Face
 * @brief Structure representing a face in a 3D mesh, defined by three vertices.
 */
struct Face {
  std::array<int, 3> vertexIndices; ///< Indices of the vertices making up the face.

  /**
   * @brief Equality operator for comparing two faces.
   * @param other The face to compare with.
   * @return True if the faces are equal, false otherwise.
   */
  bool operator==(const Face& other) const {
    return vertexIndices[0] == other.vertexIndices[0] && vertexIndices[1] == other.vertexIndices[1] &&
           vertexIndices[2] == other.vertexIndices[2];
  }
};

/**
 * @class Mesh
 * @brief Class representing a 3D mesh, containing vertices and faces.
 */
class Mesh {
private:
  std::vector<Vertex> vertices; ///< List of vertices in the mesh.
  std::vector<Face>   faces;    ///< List of faces in the mesh.

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
  const std::vector<Vertex>& getVertices() const { return vertices; }

  /**
   * @brief Retrieves a specific vertex by index.
   * @param index The index of the vertex.
   * @return A const reference to the vertex at the given index.
   */
  const Vertex& getVertex(int index) const { return vertices[index]; }

  /**
   * @brief Retrieves the list of faces in the mesh.
   * @return A const reference to the list of faces.
   */
  const std::vector<Face>& getFaces() const { return faces; }

  /**
   * @brief Equality operator for comparing two meshes.
   * @param other The mesh to compare with.
   * @return True if the meshes are equal, false otherwise.
   */
  bool operator==(const Mesh& other) const { return vertices == other.vertices && faces == other.faces; }

  ~Mesh() = default; ///< Default destructor.
};

#endif // CORE_MESH_HPP
