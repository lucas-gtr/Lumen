/**
 * @file Reinhard.hpp
 * @brief Header file for the ReinhardToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_REINHARD_HPP
#define POSTPROCESSING_TONEMAPPING_REINHARD_HPP

#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class ReinhardToneMapping
 * @brief Class implementing the Reinhard tone mapping algorithm.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of the Reinhard tone mapping algorithm.
 */
class ReinhardToneMapping : public ToneMappingStrategy {
public:
  ReinhardToneMapping() = default; ///< Default constructor.

  /**
   * @brief Applies the Reinhard tone mapping algorithm to a given value.
   * @param value The value to be tone-mapped.
   *
   * This method modifies the input value in place using the Reinhard tone mapping formula.
   * The formula is defined as:
   * \f$ \text{output} = \frac{\text{input}}{1 + \text{input}} \f$
   * This formula ensures that the output value is clamped between 0 and 1.
   */
  void apply(double& value) const override { value = value <= 0.0 ? 0.0 : value / (1.0 + value); }
};

#endif // POSTPROCESSING_TONEMAPPING_REINHARD_HPP