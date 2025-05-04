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
  NoToneMapping() = default;

  /**
   * @brief Applies no tone mapping to a given value.
   * @param value The value to be tone-mapped.
   *
   * This method modifies the input value in place by clamping it between 0 and 1.
   */
  void apply(double& value) const override { value = std::clamp(value, 0.0, 1.0); }
};

#endif // POSTPROCESSING_TONEMAPPING_NONE_HPP