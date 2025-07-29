/**
 * @file CommonTypes.hpp
 * @brief Header file for common types used in the project.
 */
#ifndef CORE_COMMONTYPES_HPP
#define CORE_COMMONTYPES_HPP

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

struct ColorRGBA; ///< Forward declaration for ColorRGBA.

/**
 * @struct ColorRGB
 * @brief Structure to hold RGB color values.
 */
struct alignas(ALIGN32) ColorRGB {
  double r = 0.0;
  double g = 0.0;
  double b = 0.0;

  ColorRGB() = default;

  ColorRGB(double red, double green, double blue) : r(red), g(green), b(blue) {}

  explicit ColorRGB(double grayscale) : r(grayscale), g(grayscale), b(grayscale) {}

  explicit ColorRGB(const ColorRGBA& color);

  ColorRGB  operator+(const ColorRGB& other) const { return {r + other.r, g + other.g, b + other.b}; }
  ColorRGB& operator+=(const ColorRGB& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }

  ColorRGBA operator+(const ColorRGBA& other) const;

  ColorRGB  operator-(const ColorRGB& other) const { return {r - other.r, g - other.g, b - other.b}; }
  ColorRGB& operator-=(const ColorRGB& other) {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
  }

  ColorRGB  operator*(double scalar) const { return {r * scalar, g * scalar, b * scalar}; }
  ColorRGB& operator*=(double scalar) {
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
  }

  ColorRGB  operator*(const ColorRGB& other) const { return {r * other.r, g * other.g, b * other.b}; }
  ColorRGB& operator*=(const ColorRGB& other) {
    r *= other.r;
    g *= other.g;
    b *= other.b;
    return *this;
  }

  ColorRGB operator/(double scalar) const {
    if(scalar == 0.0) {
      throw std::runtime_error("Division by zero in ColorRGB operator/");
    }
    return {r / scalar, g / scalar, b / scalar};
  }
  ColorRGB& operator/=(double scalar) {
    if(scalar == 0.0) {
      throw std::runtime_error("Division by zero in ColorRGB operator/=");
    }
    r /= scalar;
    g /= scalar;
    b /= scalar;
    return *this;
  }

  ColorRGB operator/(const ColorRGB& other) const {
    if(other.r == 0.0 || other.g == 0.0 || other.b == 0.0) {
      throw std::runtime_error("A component of ColorRGB is zero in operator/");
    }
    return {r / other.r, g / other.g, b / other.b};
  }
  ColorRGB& operator/=(const ColorRGB& other) {
    if(other.r == 0.0 || other.g == 0.0 || other.b == 0.0) {
      throw std::runtime_error("A component of ColorRGB is zero in operator/=");
    }
    r /= other.r;
    g /= other.g;
    b /= other.b;
    return *this;
  }

  bool operator==(const ColorRGB& other) const { return (r == other.r) && (g == other.g) && (b == other.b); }

  double maxComponent() const { return std::max({r, g, b}); }
  double minComponent() const { return std::min({r, g, b}); }

  void clamp(double min, double max) {
    r = std::clamp(r, min, max);
    g = std::clamp(g, min, max);
    b = std::clamp(b, min, max);
  }

  friend ColorRGB operator*(double scalar, const ColorRGB& color) { return color * scalar; }

  friend ColorRGB lerp(const ColorRGB& start, const ColorRGB& end, double t) { return start + (end - start) * t; }

  friend ColorRGB lerp(const ColorRGB& start, const ColorRGB& end, const ColorRGB& t) {
    return start + (end - start) * t;
  }

  friend std::ostream& operator<<(std::ostream& os, const ColorRGB& color) {
    return os << "ColorRGB(" << color.r << ", " << color.g << ", " << color.b << ")";
  }
};

/**
 * @struct ColorRGBA
 * @brief Structure to hold RGBA color values.
 */
struct alignas(ALIGN32) ColorRGBA {
  double r = 0.0;
  double g = 0.0;
  double b = 0.0;
  double a = 1.0;

  ColorRGBA() = default;

  ColorRGBA(double red, double green, double blue, double alpha) : r(red), g(green), b(blue), a(alpha) {}

  explicit ColorRGBA(double grayscale) : r(grayscale), g(grayscale), b(grayscale) {}

  ColorRGBA(double grayscale, double alpha) : r(grayscale), g(grayscale), b(grayscale), a(alpha) {}

  explicit ColorRGBA(const ColorRGB& color) : r(color.r), g(color.g), b(color.b) {}

  ColorRGBA(const ColorRGB& color, double alpha) : r(color.r), g(color.g), b(color.b), a(alpha) {}

  ColorRGBA operator+(const ColorRGB& other) const { return {r + other.r, g + other.g, b + other.b, a}; }

  ColorRGBA  operator+(const ColorRGBA& other) const { return {r + other.r, g + other.g, b + other.b, a + other.a}; }
  ColorRGBA& operator+=(const ColorRGBA& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    a += other.a;
    return *this;
  }

  ColorRGBA  operator-(const ColorRGBA& other) const { return {r - other.r, g - other.g, b - other.b, a - other.a}; }
  ColorRGBA& operator-=(const ColorRGBA& other) {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
    return *this;
  }

  ColorRGBA  operator*(double scalar) const { return {r * scalar, g * scalar, b * scalar, a * scalar}; }
  ColorRGBA& operator*=(double scalar) {
    r *= scalar;
    g *= scalar;
    b *= scalar;
    a *= scalar;
    return *this;
  }

  ColorRGBA  operator*(const ColorRGB& other) const { return {r * other.r, g * other.g, b * other.b, a}; }
  ColorRGBA& operator*=(const ColorRGB& other) {
    r *= other.r;
    g *= other.g;
    b *= other.b;
    return *this;
  }

  ColorRGBA  operator*(const ColorRGBA& other) const { return {r * other.r, g * other.g, b * other.b, a * other.a}; }
  ColorRGBA& operator*=(const ColorRGBA& other) {
    r *= other.r;
    g *= other.g;
    b *= other.b;
    a *= other.a;
    return *this;
  }

  ColorRGBA operator/(double scalar) const {
    if(scalar == 0.0) {
      throw std::runtime_error("Division by zero in ColorRGBA operator/");
    }
    return {r / scalar, g / scalar, b / scalar, a / scalar};
  }
  ColorRGBA& operator/=(double scalar) {
    if(scalar == 0.0) {
      throw std::runtime_error("Division by zero in ColorRGBA operator/=");
    }
    r /= scalar;
    g /= scalar;
    b /= scalar;
    a /= scalar;
    return *this;
  }

  ColorRGBA operator/(const ColorRGBA& other) const {
    if(other.r == 0.0 || other.g == 0.0 || other.b == 0.0 || other.a == 0.0) {
      throw std::runtime_error("Division by zero in ColorRGBA operator/");
    }
    return {r / other.r, g / other.g, b / other.b, a / other.a};
  }
  ColorRGBA& operator/=(const ColorRGBA& other) {
    if(other.r == 0.0 || other.g == 0.0 || other.b == 0.0 || other.a == 0.0) {
      throw std::runtime_error("Division by zero in ColorRGBA operator/=");
    }
    r /= other.r;
    g /= other.g;
    b /= other.b;
    a /= other.a;
    return *this;
  }

  bool operator==(const ColorRGBA& other) const {
    return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
  }

  double maxComponent() const { return std::max({r, g, b, a}); }
  double minComponent() const { return std::min({r, g, b, a}); }

  void clamp(double min, double max) {
    r = std::clamp(r, min, max);
    g = std::clamp(g, min, max);
    b = std::clamp(b, min, max);
    a = std::clamp(a, min, max);
  }

  friend ColorRGBA operator*(double scalar, const ColorRGBA& color) { return color * scalar; }

  friend ColorRGBA lerp(const ColorRGBA& start, const ColorRGBA& end, double t) { return start + (end - start) * t; }

  friend ColorRGBA lerp(const ColorRGBA& start, const ColorRGBA& end, const ColorRGBA& t) {
    return start + (end - start) * t;
  }

  friend std::ostream& operator<<(std::ostream& os, const ColorRGBA& color) {
    return os << "ColorRGBA(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
  }
};

// Definition needed after ColorRGBA full declaration
inline ColorRGB::ColorRGB(const ColorRGBA& color) : r(color.r), g(color.g), b(color.b) {}

inline ColorRGBA ColorRGB::operator+(const ColorRGBA& other) const {
  return {r + other.r, g + other.g, b + other.b, other.a};
}

#endif // CORE_COMMONTYPES_HPP
