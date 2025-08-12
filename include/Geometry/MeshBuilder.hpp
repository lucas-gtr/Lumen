/**
 * @file MeshBuilder.hpp
 * @brief Header file for the MeshBuilder class.
 */
#ifndef GEOMETRY_MESHBUILDER_HPP
#define GEOMETRY_MESHBUILDER_HPP

class Mesh;

/**
 * @class MeshBuilder
 * @brief Abstract base class for building meshes.
 *
 * This class provides a pure virtual function `build()` that must be implemented
 * by derived classes to create specific types of meshes. It serves as a base class
 * for different mesh builders.
 */
class MeshBuilder {
public:
  MeshBuilder() = default; ///< Default constructor.

  MeshBuilder(const MeshBuilder&)            = delete;
  MeshBuilder& operator=(const MeshBuilder&) = delete;
  MeshBuilder(MeshBuilder&&)                 = delete;
  MeshBuilder& operator=(MeshBuilder&&)      = delete;

  /**
   * @brief Pure virtual function to build a mesh.
   * @return A Mesh object representing the built mesh.
   */
  virtual Mesh build() const = 0;

  virtual ~MeshBuilder() = default; ///< Default destructor.
};

#endif // GEOMETRY_MESHBUILDER_HPP
