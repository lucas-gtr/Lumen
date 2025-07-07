/**
 * @file BVHBuilder.hpp
 * @brief Header file for the BVH builder functions.
 */
#ifndef BVH_BVHBUILDER_HPP
#define BVH_BVHBUILDER_HPP

#include <memory>
#include <vector>

#include "BVH/BVHNode.hpp"
#include <linalg/Vec3.hpp>

/**
 * @namespace BVH
 * @brief Namespace for Bounding Volume Hierarchy (BVH) related functions.
 */
namespace BVH {
/**
 * @brief Gets the largest axis of a given extent.
 * @param extent The extent vector representing the size in each axis (x, y, z).
 * @return The index of the largest axis (0 for x, 1 for y, 2 for z).
 */
int getLargestAxis(const linalg::Vec3d& extent);

/**
 * @brief Builds a BVH node (left and right child) from a list of objects.
 * @param node The  node to be constructed.
 * @param objects A vector of shared pointers to BVHNode objects representing the objects to be included in the BVH.
 * @param start The starting index in the objects vector.
 * @param end The ending index in the objects vector.
 */
void constructNode(std::shared_ptr<BVHNode>& node, std::vector<std::shared_ptr<BVHNode>>& objects, int start, int end);
}; // namespace BVH

#endif // BVH_BVHBUILDER_HPP