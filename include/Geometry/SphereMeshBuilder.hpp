/**
 * @file SphereMeshBuilder.hpp
 * @brief Header file for the SphereMeshBuilder class.
 */
#ifndef GEOMETRY_SPHEREMESHBUILDER_HPP
#define GEOMETRY_SPHEREMESHBUILDER_HPP

#include "Geometry/Mesh.hpp"
#include "Geometry/MeshBuilder.hpp"

/**
 * @class SphereMeshBuilder
 * @brief A class for building a sphere mesh.
 *
 * This class is responsible for creating a sphere mesh with the specified radius, segments, and rings.
 * It inherits from the MeshBuilder class and implements the build method to
 * generate the sphere mesh.
 */
class SphereMeshBuilder : public MeshBuilder {
private:
  double m_radius;
  int    m_segments;
  int    m_rings;

public:
  /**
   * @brief Constructs a SphereMeshBuilder with the specified radius, segments, and rings.
   *
   * @param radius The radius of the sphere.
   * @param segments The number of segments in the sphere.
   * @param rings The number of rings in the sphere.
   */
  explicit SphereMeshBuilder(double radius, int segments, int rings);

  /**
   * @brief Builds a sphere mesh with the specified parameters.
   * @return A Mesh object representing the sphere.
   */
  Mesh build() const override;
};

#endif // GEOMETRY_SPHEREMESHBUILDER_HPP
