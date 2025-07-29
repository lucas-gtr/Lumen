/**
 * @file ACES.hpp
 * @brief Header file for the AcesToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_ACES_HPP
#define POSTPROCESSING_TONEMAPPING_ACES_HPP

#include <algorithm>
#include <linalg/Mat3.hpp>
#include <linalg/Vec3.hpp>
#include <linalg/linalg.hpp>

#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class AcesToneMapping
 * @brief Class implementing the ACES tone mapping algorithm.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of the ACES tone mapping algorithm, which is designed
 * to convert high dynamic range (HDR) images to low dynamic range (LDR)
 * images while preserving the appearance of the original scene.
 */
class AcesToneMapping : public ToneMappingStrategy {
public:
  AcesToneMapping() = default;

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  /**
   * @brief Applies the ACES tone mapping algorithm to a given color.
   * @param color The input color to be converted.
   * @return The converted color in LDR format.
   */
  static linalg::Vec3d RttAndOdtFit(const linalg::Vec3d& color) {
    const linalg::Vec3d a =
        linalg::cwiseProduct(color, (color + linalg::Vec3d(0.0245786))) - linalg::Vec3d(0.000090537);
    const linalg::Vec3d b =
        linalg::cwiseProduct(color, (0.983729 * color + linalg::Vec3d(0.4329510))) + linalg::Vec3d(0.238081);
    return linalg::cwiseProduct(a, b.cwiseInverse());
  }
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  /**
   * @brief Fits the input color to the ACES color space and applies the output device transform.
   * @param color The input color to be converted.
   * @return The converted color in ACES color space.
   */
  linalg::Vec3d acesFitted(const linalg::Vec3d& color) const {
    linalg::Vec3d v = aces_input * color;
    v               = RttAndOdtFit(v);
    return aces_output * v;
  }

  /**
   * @brief Converts a value to LDR using the ACES tone mapping algorithm.
   * @param value The value to convert.
   * @return The converted value in LDR format, clamped between 0 and 1.
   */
  double convertToLDR(double value) const override {
    const linalg::Vec3d vec_color(value);
    const linalg::Vec3d aces_color = acesFitted(vec_color);
    return (aces_color.x + aces_color.y + aces_color.z) / 3;
  }

  /**
   * @brief Converts a ColorRGB object to LDR using the ACES tone mapping algorithm.
   * @param color The ColorRGB object to convert.
   * @return The converted ColorRGB object in LDR format.
   */
  ColorRGB convertToLDR(const ColorRGB& color) const override {
    const linalg::Vec3d vec_color(color.r, color.g, color.b);
    const linalg::Vec3d aces_color = acesFitted(vec_color);
    return {aces_color.x, aces_color.y, aces_color.z};
  }

private:
  const linalg::Mat3d aces_input = {
      {0.59719, 0.35458, 0.04823}, {0.07600, 0.90834, 0.01566}, {0.02840, 0.13383, 0.83777}};

  const linalg::Mat3d aces_output = {
      {1.60475, -0.53108, -0.07367}, {-0.10208, 1.10813, -0.00605}, {-0.00327, -0.07276, 1.07602}};
};

#endif // POSTPROCESSING_TONEMAPPING_ACES_HPP