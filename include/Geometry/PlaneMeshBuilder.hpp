/**
 * @file PlaneMeshBuilder.hpp
 * @brief Header file for the PlaneMeshBuilder class.
 */
#ifndef GEOMETRY_PLANEMESHBUILDER_HPP
#define GEOMETRY_PLANEMESHBUILDER_HPP

#include "Geometry/MeshBuilder.hpp"

class Mesh;

/**
 * @class PlaneMeshBuilder
 * @brief A class for building a plane mesh.
 *
 * This class is responsible for creating a plane mesh with the specified width and length.
 * It inherits from the MeshBuilder class and implements the build method to
 * generate the plane mesh.
 */
class PlaneMeshBuilder : public MeshBuilder {
private:
  double m_width;
  double m_length;

public:
  /**
   * @brief Constructs a PlaneMeshBuilder with specified width and length.
   * @param width The width of the plane.
   * @param length The length of the plane.
   */
  explicit PlaneMeshBuilder(double width, double length);

  /**
   * @brief Builds a plane mesh with the specified dimensions.
   * @return A Mesh object representing the plane.
   */
  Mesh build() const override;
};

#endif // GEOMETRY_PLANEMESHBUILDER_HPP