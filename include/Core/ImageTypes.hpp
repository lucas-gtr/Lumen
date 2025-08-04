/**
 * @file ImageTypes.hpp
 * @brief Header file for image types used in the project.
 */
#ifndef CORE_IMAGETYPES_HPP
#define CORE_IMAGETYPES_HPP

#include <algorithm>
#include <cstdint>
#include <immintrin.h>
#include <iostream>

#include "Core/Config.hpp"

/**
 * @struct ImageProperties
 * @brief Structure to hold image properties such as width, height, and number of channels.
 */
struct ImageProperties {
  int width    = 1;
  int height   = 1;
  int channels = 1;

  std::uint64_t bufferSize() const {
    return static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height) *
           static_cast<std::uint64_t>(channels);
  }
};

/**
 * @struct PixelCoord
 * @brief Structure to hold pixel coordinates (x, y) in the framebuffer.
 * @note The coordinates are zero-based, meaning (0, 0) is the top-left corner of the image.
 */
struct alignas(ALIGN8) PixelCoord {
  int x = 0; ///< X coordinate of the pixel.
  int y = 0; ///< Y coordinate of the pixel.
};

/**
 * @struct Resolution
 * @brief Structure to hold the resolution of an image.
 */
struct alignas(ALIGN8) Resolution {
  int width  = 1; ///< Width of the resolution.
  int height = 1; ///< Height of the resolution.
};

/**
 * @struct TextureUV
 * @brief Structure to hold the UV coordinates of a texture.
 * @note The coordinates are normalized, meaning (0, 0) is the top-left corner and (1, 1) is the bottom-right corner.
 */
struct alignas(ALIGN16) TextureUV {
  double u = 0.0; ///< U coordinate of the texture.
  double v = 0.0; ///< V coordinate of the texture.
};

#endif // CORE_IMAGETYPES_HPP
