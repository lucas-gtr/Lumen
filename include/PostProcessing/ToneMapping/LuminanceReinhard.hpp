/**
 * @file LuminanceReinhard.hpp
 * @brief Header file for the LuminanceReinhardToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_LUMINANCEREINHARD_HPP
#define POSTPROCESSING_TONEMAPPING_LUMINANCEREINHARD_HPP

#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class LuminanceReinhardToneMapping
 * @brief Class implementing the Reinhard tone mapping algorithm based on luminance.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of the Reinhard tone mapping algorithm that
 * adjusts the color values based on luminance.
 */
class LuminanceReinhardToneMapping : public ToneMappingStrategy {
public:
  LuminanceReinhardToneMapping() = default; ///< Default constructor.

  /**
   * @brief Converts a value to LDR using the Reinhard tone mapping algorithm based on luminance.
   * @param value The value to convert.
   * @return The converted value in LDR format.
   */
  double convertToLDR(double value) const override {
    if(value <= 0.0) {
      return 0.0;
    }
    return value / (1.0 + value);
  }

  /**
   * @brief Converts a ColorRGB object to LDR using the Reinhard tone mapping algorithm based on luminance.
   * @param color The ColorRGB object to convert.
   * @return The converted ColorRGB object in LDR format.
   */
  ColorRGB convertToLDR(const ColorRGB& color) const override {
    const double   luminance = getLuminance(color);
    const ColorRGB ldr_color = color / (ColorRGB(1.0) + color);
    return lerp(color / (1.0 + luminance), ldr_color, ldr_color);
  }
};

#endif // POSTPROCESSING_TONEMAPPING_LUMINANCEREINHARD_HPP