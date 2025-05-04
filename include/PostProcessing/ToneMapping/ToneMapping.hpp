/**
 * @file ToneMapping.hpp
 * @brief Header file for the ToneMappingStrategy class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_TONEMAPPING_HPP
#define POSTPROCESSING_TONEMAPPING_TONEMAPPING_HPP

#include "ImplementationParameters/Parameters.hpp"

/**
 * @class ToneMappingStrategy
 * @brief Abstract base class for tone mapping strategies.
 *
 * This class defines the interface for different tone mapping algorithms.
 * Derived classes should implement the apply method to perform the specific tone mapping operation.
 */
class ToneMappingStrategy {
public:
  ToneMappingStrategy() = default; ///< Default constructor.

  ToneMappingStrategy(const ToneMappingStrategy&)            = delete;
  ToneMappingStrategy& operator=(const ToneMappingStrategy&) = delete;
  ToneMappingStrategy(ToneMappingStrategy&&)                 = delete;
  ToneMappingStrategy& operator=(ToneMappingStrategy&&)      = delete;

  /**
   * @brief Applies the tone mapping algorithm to a given value.
   * @param value The value to be tone-mapped.
   */
  virtual void apply(double& value) const = 0;

  /**
   * @brief Updates the parameters for the tone mapping algorithm.
   * @param parameters The parameters to be updated.
   */
  virtual void updateParameters(Parameters* parameters) { (void)parameters; }

  /**
   * @brief Virtual destructor for the ToneMappingStrategy class.
   */
  virtual ~ToneMappingStrategy() = default;
};

#endif // POSTPROCESSING_TONEMAPPING_TONEMAPPING_HPP