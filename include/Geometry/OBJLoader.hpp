/**
 * @file OBJLoader.hpp
 * @brief Header file for the OBJLoader class.
 */
#ifndef GEOMETRY_OBJLOADER_HPP
#define GEOMETRY_OBJLOADER_HPP

#include <Eigen/Core>
#include <string>
#include <unordered_map>
#include <vector>

#include "Core/CommonTypes.hpp"
#include "Geometry/Mesh.hpp"

/**
 * @class OBJLoader
 * @brief Class for loading 3D models from OBJ files.
 *
 * This class provides functionality to load 3D models from OBJ files and parse their vertices,
 * normals, texture coordinates, and faces. It constructs a Mesh object representing the loaded model.
 */
class OBJLoader {
private:
  static void parseVertexLine(const std::string& line, std::vector<Eigen::Vector3d>& positions);
  static void parseNormalLine(const std::string& line, std::vector<Eigen::Vector3d>& normals);
  static void parseUVLine(const std::string& line, std::vector<TextureUV>& uvs);
  static Face parseFaceLine(const std::string& line, const std::vector<Eigen::Vector3d>& positions,
                            const std::vector<Eigen::Vector3d>& normals, const std::vector<TextureUV>& uvs,
                            std::vector<Vertex>& vertices, std::unordered_map<std::string, int>& vertexMap);

public:
  /**
   * @brief Loads a mesh from an OBJ file.
   * @param filename The path to the OBJ file.
   * @return A Mesh object representing the loaded mesh.
   */
  static Mesh load(const std::string& filename);
};

#endif // GEOMETRY_OBJLOADER_HPP