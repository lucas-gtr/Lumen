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
#include "ImplementationParameters/ParameterTypes.hpp"
#include "ImplementationParameters/Parameters.hpp"
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
   * @param parameters Pointer to the parameters object.
   *
   * This constructor initializes the exposure parameter with a default value.
   */
  explicit ExposureToneMapping(Parameters* parameters) {
    parameters->setDefaultParameter("exposure", DEFAULT_TONE_MAPPING_EXPOSURE);
  }

  /**
   * @brief Updates the exposure parameter.
   * @param parameters Pointer to the parameters object.
   */
  void updateParameters(Parameters* parameters) override {
    const DoubleParam* exposure_param = parameters->get<DoubleParam>("exposure");
    if(exposure_param == nullptr) {
      std::cerr << "Exposure parameter not found. Using default value." << '\n';
      return;
    }
    m_exposure = std::clamp(exposure_param->getValue(), MIN_TONE_MAPPING_EXPOSURE, MAX_TONE_MAPPING_EXPOSURE);
  }

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