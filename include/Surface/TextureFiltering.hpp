/**
 * @file TextureFiltering.hpp
 * @brief Header file for texture filtering functions.
 */
#ifndef SURFACE_TEXTUREFILTERING_HPP
#define SURFACE_TEXTUREFILTERING_HPP

#include <cstdint>

#include "Core/CommonTypes.hpp"

namespace TextureSampling {

/**
 * @enum TextureFiltering
 * @brief Enumeration for texture filtering modes.
 *
 * This enum defines the filtering modes for textures, including NEAREST and LINEAR.
 */
enum class TextureFiltering : std::uint8_t { NEAREST, BILINEAR };

/**
 * @struct BilinearSampleInfo
 * @brief Structure to hold bilinear sampling information.
 *
 * This structure contains the coordinates of the four surrounding pixels and the
 * fractional offsets for bilinear sampling.
 */
struct BilinearSampleInfo {
  int    x0, x1, y0, y1;
  double dx, dy;
};

/**
 * @brief Samples a texture using nearest-neighbor filtering.
 *
 * This function takes UV coordinates and resolution, and returns the pixel coordinates
 * of the nearest pixel in the texture.
 *
 * @param uv_coord The UV coordinates to sample.
 * @param resolution The resolution of the texture.
 * @return The pixel coordinates of the nearest pixel.
 */
PixelCoord sampleNearest(TextureUV uv_coord, Resolution resolution);

/**
 * @brief Samples a texture using bilinear filtering.
 *
 * This function takes UV coordinates and resolution, and returns the bilinear sampling
 * information, including the coordinates of the four surrounding pixels and the
 * fractional offsets.
 *
 * @param uv_coord The UV coordinates to sample.
 * @param resolution The resolution of the texture.
 * @return The bilinear sampling information.
 */
BilinearSampleInfo sampleBilinear(TextureUV uv_coord, Resolution resolution);

} // namespace TextureSampling

#endif // SURFACE_TEXTUREFILTERING_HPP