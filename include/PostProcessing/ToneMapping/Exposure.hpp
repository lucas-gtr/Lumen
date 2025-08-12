/**
 * @file Exposure.hpp
 * @brief Header file for the ExposureToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_EXPOSURE_HPP
#define POSTPROCESSING_TONEMAPPING_EXPOSURE_HPP

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Core/Config.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class ExposureToneMapping
 * @brief Class implementing the exposure tone mapping algorithm.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of the exposure tone mapping algorithm.
 */
class ExposureToneMapping : public ToneMappingStrategy {
private:
  double m_exposure = 1.0;

  double convertToLDR(double value) const {
    if(value <= 0.0) {
      return 0.0;
    }
    return 1.0 - std::exp(-value * m_exposure);
  }

public:
  /**
   * @brief Constructor for the ExposureToneMapping class.
   * @param exposure The exposure value to be used in the tone mapping algorithm.
   */
  explicit ExposureToneMapping(double exposure) : m_exposure(exposure) {}

  /**
   * @brief Converts a ColorRGB object to LDR using the exposure tone mapping algorithm.
   * @param color The ColorRGB object to convert.
   * @return The converted ColorRGB object in LDR format.
   */
  ColorRGB convertToLDR(const ColorRGB& color) const override {
    return {convertToLDR(color.r), convertToLDR(color.g), convertToLDR(color.b)};
  }
};

#endif // POSTPROCESSING_TONEMAPPING_EXPOSURE_HPP