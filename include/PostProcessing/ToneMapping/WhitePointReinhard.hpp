/**
 * @file WhitePointReinhard.hpp
 * @brief Header file for the WhitePointReinhardToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_WHITEPOINTREINHARD_HPP
#define POSTPROCESSING_TONEMAPPING_WHITEPOINTREINHARD_HPP

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Core/Config.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class WhitePointReinhardToneMapping
 * @brief Class implementing the Reinhard tone mapping algorithm with white point.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of the Reinhard tone mapping algorithm that
 * adjusts the color values based on a specified white point.
 */
class WhitePointReinhardToneMapping : public ToneMappingStrategy {
private:
  double m_white_point = 1.0;

public:
  /**
   * @brief Constructor for the WhitePointReinhardToneMapping class.
   * @param white_point The white point value to use for tone mapping.
   */
  explicit WhitePointReinhardToneMapping(double white_point) : m_white_point(white_point) {}

  /**
   * @brief Converts a value to LDR using the Reinhard tone mapping algorithm with white point.
   * @param value The value to convert.
   * @return The converted value in LDR format.
   */
  double convertToLDR(double value) const override {
    const double numerator = (1.0 + value / (m_white_point * m_white_point));
    return value * numerator / (1.0 + value);
  }

  /**
   * @brief Converts a ColorRGB object to LDR using the Reinhard tone mapping algorithm with white point.
   * @param color The ColorRGB object to convert.
   * @return The converted ColorRGB object in LDR format.
   */
  ColorRGB convertToLDR(const ColorRGB& color) const override {
    const double current_luminance = getLuminance(color);
    const double numerator         = (1.0 + current_luminance / (m_white_point * m_white_point));
    return color * numerator / (1.0 + current_luminance);
  }
};

#endif // POSTPROCESSING_TONEMAPPING_WHITEPOINTREINHARD_HPP