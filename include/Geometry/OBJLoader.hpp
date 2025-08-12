/**
 * @file OBJLoader.hpp
 * @brief Header file for the OBJLoader namespace.
 */
#ifndef GEOMETRY_OBJLOADER_HPP
#define GEOMETRY_OBJLOADER_HPP

#include <string>

class Mesh;

/**
 * @namespace OBJLoader
 * @brief Namespace for loading 3D meshes from OBJ files.
 */
namespace OBJLoader {
/**
 * @brief Loads a 3D mesh from an OBJ file.
 * @param filename The name of the OBJ file to load.
 * @return A Mesh object containing the loaded geometry.
 */
Mesh load(const std::string& filename);
} // namespace OBJLoader

#endif // GEOMETRY_OBJLOADER_HPP