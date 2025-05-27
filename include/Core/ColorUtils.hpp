/**
 * @file ColorUtils.hpp
 * @brief Header file for color utility functions.
 */
#ifndef CORE_COLORUTILS_HPP
#define CORE_COLORUTILS_HPP

#include <cmath>

#include "CommonTypes.hpp"

static constexpr double NORMALIZED_TO_COLOR8 = 255.999;
static constexpr int    COLOR8_MAX_VALUE     = 255;
static constexpr double COLOR8_TO_NORMALIZED = 1.0 / COLOR8_MAX_VALUE;

static constexpr double RED_CHANNEL   = 0.299;
static constexpr double GREEN_CHANNEL = 0.587;
static constexpr double BLUE_CHANNEL  = 0.114;

// Constants for sRGB to Linear and Linear to sRGB conversion
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
 * @brief Converts a color represented as a 3D vector to grayscale.
 * @param color The color vector (r, g, b).
 * @return The grayscale value.
 */
inline double toGrayscale(const ColorRGB& color) {
  return RED_CHANNEL * color.r + GREEN_CHANNEL * color.g + BLUE_CHANNEL * color.b;
}

/**
 * @brief Converts a color represented as a 4D vector to grayscale.
 * @param color The color vector (r, g, b, a).
 * @return The grayscale value.
 */
inline double toGrayscale(const ColorRGBA& color) {
  return RED_CHANNEL * color.r + GREEN_CHANNEL * color.g + BLUE_CHANNEL * color.b;
}

/**
 * @brief Converts a grayscale value to an RGB color.
 * @param grayscale The grayscale value.
 * @return The RGB color vector (r, g, b).
 */
inline ColorRGB toRGB(double grayscale) { return {grayscale, grayscale, grayscale}; }

/**
 * @brief Converts a color represented as a 4D vector to an RGB color.
 * @param color The color vector (r, g, b, a).
 * @return The RGB color vector (r, g, b).
 */
inline ColorRGB toRGB(const ColorRGBA& color) { return {color.r, color.g, color.b}; }

/**
 * @brief Converts a grayscale value to an RGBA color.
 * @param grayscale The grayscale value.
 * @return The RGBA color vector (r, g, b, a).
 */
inline ColorRGBA toRGBA(double grayscale) { return {grayscale, grayscale, grayscale, 1.0}; }

/**
 * @brief Converts a color represented as a 3D vector to an RGBA color.
 * @param color The color vector (r, g, b).
 * @return The RGBA color vector (r, g, b, a).
 */
inline ColorRGBA toRGBA(const ColorRGB& color) { return {color.r, color.g, color.b, 1.0}; }

/**
 * @brief Converts a value from linear space to sRGB space.
 * @param value The linear space value.
 */
inline void convertToSRGBSpace(double& value) {
  if(value <= RGB_THRESHOLD) {
    value *= RGB_THRESHOLD_FACTOR;
    return;
  }
  value = (std::pow(value, RGB_GAMMA) * RGB_FACTOR) - COLOR_SPACE_ADDENDUM;
}

/**
 * @brief Converts a value from sRGB to linear space.
 * @param value The sRGB value.
 */
inline void convertToLinearSpace(double& value) {
  if(value <= SRGB_THRESHOLD) {
    value *= SRGB_THRESHOLD_FACTOR;
    return;
  }
  value = (std::pow((value + COLOR_SPACE_ADDENDUM) * SRGB_FACTOR, SRGB_GAMMA));
}

#endif // CORE_COLORUTILS_HPP