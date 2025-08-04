/**
 * @file Color.hpp
 * @brief Header file for color utility.
 */
#ifndef CORE_COLOR_HPP
#define CORE_COLOR_HPP

#include <algorithm>
#include <cmath>
#include <immintrin.h>
#include <iostream>

#include "Core/Config.hpp"

static constexpr double NORMALIZED_TO_COLOR8 = 255.999;
static constexpr int    COLOR8_MAX_VALUE     = 255;
static constexpr double COLOR8_TO_NORMALIZED = 1.0 / NORMALIZED_TO_COLOR8;

static constexpr double GRAY_RED_CHANNEL   = 0.299;
static constexpr double GRAY_GREEN_CHANNEL = 0.587;
static constexpr double GRAY_BLUE_CHANNEL  = 0.114;

static constexpr double LUMINANCE_RED_CHANNEL   = 0.2126;
static constexpr double LUMINANCE_GREEN_CHANNEL = 0.7152;
static constexpr double LUMINANCE_BLUE_CHANNEL  = 0.0722;

static constexpr double SRGB_GAMMA            = 2.4;
static constexpr double RGB_GAMMA             = 1.0 / SRGB_GAMMA;
static constexpr double RGB_THRESHOLD         = 0.0031308;
static constexpr double SRGB_THRESHOLD        = 0.04045;
static constexpr double RGB_THRESHOLD_FACTOR  = 12.92;
static constexpr double SRGB_THRESHOLD_FACTOR = 1.0 / RGB_THRESHOLD_FACTOR;
static constexpr double RGB_FACTOR            = 1.055;
static constexpr double SRGB_FACTOR           = 1 / RGB_FACTOR;
static constexpr double COLOR_SPACE_ADDENDUM  = 0.055;

/**
 * @brief Converts a value from sRGB to linear space.
 * @param value The sRGB value.
 */
template <typename T> inline void convertToLinearSpace(T& value) {
  if(value <= SRGB_THRESHOLD) {
    value *= SRGB_THRESHOLD_FACTOR;
    return;
  }
  value = (std::pow((value + COLOR_SPACE_ADDENDUM) * SRGB_FACTOR, SRGB_GAMMA));
}

/**
 * @brief Converts a value from linear space to sRGB space.
 * @param value The linear space value.
 */
template <typename T> inline void convertToSRGBSpace(T& value) {
  if(value <= RGB_THRESHOLD) {
    value *= RGB_THRESHOLD_FACTOR;
    return;
  }
  value = (std::pow(value, RGB_GAMMA) * RGB_FACTOR) - COLOR_SPACE_ADDENDUM;
}

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

  ColorRGB toLinearSpace() const {
    ColorRGB linear_color = *this;
    convertToLinearSpace(linear_color.r);
    convertToLinearSpace(linear_color.g);
    convertToLinearSpace(linear_color.b);
    return linear_color;
  }

  ColorRGB toSRGBSpace() const {
    ColorRGB srgb_color = *this;
    convertToSRGBSpace(srgb_color.r);
    convertToSRGBSpace(srgb_color.g);
    convertToSRGBSpace(srgb_color.b);
    return srgb_color;
  }

  double grayscale() const { return (r * GRAY_RED_CHANNEL) + (g * GRAY_GREEN_CHANNEL) + (b * GRAY_BLUE_CHANNEL); }

  ColorRGBA toRGBA() const;

  double luminance() const {
    return (r * LUMINANCE_RED_CHANNEL) + (g * LUMINANCE_GREEN_CHANNEL) + (b * LUMINANCE_BLUE_CHANNEL);
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

  double grayscale() const { return (r * GRAY_RED_CHANNEL) + (g * GRAY_GREEN_CHANNEL) + (b * GRAY_BLUE_CHANNEL); }

  ColorRGB toRGB() const { return {r, g, b}; }

  double luminance() const {
    return (r * LUMINANCE_RED_CHANNEL) + (g * LUMINANCE_GREEN_CHANNEL) + (b * LUMINANCE_BLUE_CHANNEL);
  }

  ColorRGBA toLinearSpace() const {
    ColorRGBA linear_color = *this;
    convertToLinearSpace(linear_color.r);
    convertToLinearSpace(linear_color.g);
    convertToLinearSpace(linear_color.b);
    return linear_color;
  }

  ColorRGBA toSRGBSpace() const {
    ColorRGBA srgb_color = *this;
    convertToSRGBSpace(srgb_color.r);
    convertToSRGBSpace(srgb_color.g);
    convertToSRGBSpace(srgb_color.b);
    return srgb_color;
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

inline ColorRGBA ColorRGB::toRGBA() const { return {r, g, b, 1.0}; }

#endif // CORE_COLOR_HPP