/**
 * @file CommonTypes.hpp
 * @brief Header file for common types used in the project.
 */
#ifndef CORE_COMMONTYPES_HPP
#define CORE_COMMONTYPES_HPP

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

  /**
   * @brief Gets the size of the array of the framebuffer.
   * @return The size of the framebuffer.
   * @note The size is calculated as width * height * channels.
   */
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
struct alignas(Align8) PixelCoord {
  int x = 0; ///< X coordinate of the pixel.
  int y = 0; ///< Y coordinate of the pixel.
};

/**
 * @struct Resolution
 * @brief Structure to hold the resolution of an image.
 */
struct alignas(Align8) Resolution {
  int width  = 1; ///< Width of the resolution.
  int height = 1; ///< Height of the resolution.
};

/**
 * @struct TextureUV
 * @brief Structure to hold the UV coordinates of a texture.
 * @note The coordinates are normalized, meaning (0, 0) is the top-left corner and (1, 1) is the bottom-right corner.
 */
struct alignas(Align16) TextureUV {
  double u = 0.0; ///< U coordinate of the texture.
  double v = 0.0; ///< V coordinate of the texture.
};

struct ColorRGBA; ///< Forward declaration for ColorRGBA.

/**
 * @struct ColorRGB
 * @brief Structure to hold RGB color values.
 */
struct alignas(Align32) ColorRGB {
  double r = 0.0; ///< Red component of the color
  double g = 0.0; ///< Green component of the color
  double b = 0.0; ///< Blue component of the color

  ColorRGB() = default;
  ColorRGB(double red, double green, double blue);
  explicit ColorRGB(double grayscale);
  explicit ColorRGB(const ColorRGBA& color); // Ne pas définir ici

  ColorRGB  operator+(const ColorRGB& other) const;
  ColorRGB& operator+=(const ColorRGB& other);
  ColorRGB  operator*(double scalar) const;

  bool operator==(const ColorRGB& other) const;

  friend ColorRGB operator*(double scalar, const ColorRGB& color);

  friend std::ostream& operator<<(std::ostream& os, const ColorRGB& color) {
    return os << "ColorRGB(" << color.r << ", " << color.g << ", " << color.b << ")";
  }
};

/**
 * @struct ColorRGBA
 * @brief Structure to hold RGBA color values.
 */
struct alignas(Align32) ColorRGBA {
  double r = 0.0; ///< Red component of the color
  double g = 0.0; ///< Green component of the color
  double b = 0.0; ///< Blue component of the color
  double a = 1.0; ///< Alpha component of the color (opacity)

  ColorRGBA() = default;
  ColorRGBA(double red, double green, double blue, double alpha);
  explicit ColorRGBA(double grayscale);
  ColorRGBA(double grayscale, double alpha);
  explicit ColorRGBA(const ColorRGB& color);
  ColorRGBA(const ColorRGB& color, double alpha);

  ColorRGBA operator+(const ColorRGBA& other) const;
  ColorRGBA operator*(double scalar) const;

  bool operator==(const ColorRGBA& other) const;

  friend ColorRGBA operator*(double scalar, const ColorRGBA& color);

  friend std::ostream& operator<<(std::ostream& os, const ColorRGBA& color) {
    return os << "ColorRGBA(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
  }
};

#endif // CORE_COMMONTYPES_HPP