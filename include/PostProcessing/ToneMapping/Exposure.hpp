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
  double m_exposure = DEFAULT_TONE_MAPPING_EXPOSURE;

public:
  /**
   * @brief Constructor for the ExposureToneMapping class.
   * @param double exposure The exposure value to be used in the tone mapping algorithm.
   */
  explicit ExposureToneMapping(double exposure) : m_exposure(exposure) {}

  /**
   * @brief Applies the exposure tone mapping algorithm to a given value.
   * @param value The value to be tone-mapped.
   *
   * This method modifies the input value in place using the exposure tone mapping formula.
   * The formula is defined as:
   * \f$ \text{output} = 1 - e^{-\text{input} \cdot \text{exposure}} \f$
   *
   * This formula ensures that the output value is clamped between 0 and 1.
   */
  void apply(double& value) const override { value = 1.0 - std::exp(-value * m_exposure); }
};

#endif // POSTPROCESSING_TONEMAPPING_EXPOSURE_HPP