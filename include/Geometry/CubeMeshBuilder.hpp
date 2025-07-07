/**
 * @file CubeMeshBuilder.hpp
 * @brief Header file for the CubeMeshBuilder class.
 */
#ifndef GEOMETRY_CUBEMESHBUILDER_HPP
#define GEOMETRY_CUBEMESHBUILDER_HPP

#include "Geometry/Mesh.hpp"
#include "Geometry/MeshBuilder.hpp"

/**
 * @class CubeMeshBuilder
 * @brief A class for building a cube mesh.
 *
 * This class is responsible for creating a cube mesh with the specified size.
 * It inherits from the MeshBuilder class and implements the build method to
 * generate the cube mesh.
 */
class CubeMeshBuilder : public MeshBuilder {
private:
  double m_size;

  constexpr static int VERTICES_PER_FACE       = 4;
  constexpr static int TOTAL_VERTICES          = 24;
  constexpr static int TOTAL_FACES             = 6;
  constexpr static int TOTAL_VERTICES_POSITION = 8;

public:
  /**
   * @brief Constructs a CubeMeshBuilder with the specified size.
   *
   * @param size The size of the cube.
   */
  explicit CubeMeshBuilder(double size);

  /**
   * @brief Builds a cube mesh.
   * @return A Mesh object representing the cube.
   */
  Mesh build() const override;
};

#endif // GEOMETRY_CUBEMESHBUILDER_HPP
