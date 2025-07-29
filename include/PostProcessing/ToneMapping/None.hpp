/**
 * @file None.hpp
 * @brief Header file for the NoToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_NONE_HPP
#define POSTPROCESSING_TONEMAPPING_NONE_HPP

#include <algorithm>

#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class NoToneMapping
 * @brief Class implementing no tone mapping strategy.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of a no tone mapping strategy, which simply clamps
 * the input value between 0 and 1.
 */
class NoToneMapping : public ToneMappingStrategy {
public:
  NoToneMapping() = default; ///< Default constructor.

  /**
   * @brief Converts a value to LDR without any tone mapping.
   * @param value The value to convert.
   * @return The converted value in LDR format, clamped between 0 and
   */
  double convertToLDR(double value) const override { return std::clamp(value, 0.0, 1.0); }

  /**
   * @brief Converts a ColorRGB object to LDR without any tone mapping.
   * @param color The ColorRGB object to convert.
   * @return The converted ColorRGB object in LDR format, with each component clamped between 0 and 1.
   */
  ColorRGB convertToLDR(const ColorRGB& color) const override {
    return {std::clamp(color.r, 0.0, 1.0), std::clamp(color.g, 0.0, 1.0), std::clamp(color.b, 0.0, 1.0)};
  }
};

#endif // POSTPROCESSING_TONEMAPPING_NONE_HPP