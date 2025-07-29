/**
 * @file Uncharted2.hpp
 * @brief Header file for the Uncharted2ToneMapping class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_UNCHARTED2_HPP
#define POSTPROCESSING_TONEMAPPING_UNCHARTED2_HPP

#include <algorithm>

#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class Uncharted2ToneMapping
 * @brief Class implementing the Uncharted2 tone mapping algorithm.
 *
 * This class inherits from the ToneMappingStrategy class and provides
 * an implementation of the Uncharted2 tone mapping algorithm.
 */
class Uncharted2ToneMapping : public ToneMappingStrategy {
public:
  /**
   * @brief Constructor for the Uncharted2ToneMapping class.
   * @param exposure The exposure value to be used in the tone mapping algorithm.
   * @param white_point The white point value to be used in the tone mapping algorithm.
   */
  Uncharted2ToneMapping(double exposure, double white_point) : m_exposure(exposure), m_white_point(white_point) {}

  /**
   * @brief Gets the Uncharted2 tone mapping partial value for a given input.
   * @param value The input value to be processed.
   * @return The processed value after applying the Uncharted2 tone mapping partial.
   */
  static double GetUncharted2ToneMapPartial(double value) {
    return ((value * (A * value + B * C) + D * E) / (value * (A * value + B) + D * F)) - E / F;
  }

  /**
   * @brief Gets the Uncharted2 tone mapping partial value for a given ColorRGB input.
   * @param color The input ColorRGB to be processed.
   * @return The processed ColorRGB after applying the Uncharted2 tone mapping partial.
   */
  static ColorRGB GetUncharted2ToneMapPartial(const ColorRGB& color) {
    return ((color * (A * color + ColorRGB(B * C)) + ColorRGB(D * E)) /
            (color * (A * color + ColorRGB(B)) + ColorRGB(D * F))) -
           ColorRGB(E / F);
  }

  /**
   * @brief Converts a value to LDR using the Uncharted2 tone mapping algorithm.
   * @param value The value to convert.
   * @return The converted value in LDR format.
   */
  double convertToLDR(double value) const override {
    const double mapped_value = GetUncharted2ToneMapPartial(value);

    const double white_scale = 1.0 / GetUncharted2ToneMapPartial(m_white_point);
    return mapped_value * white_scale;
  }

  /**
   * @brief Converts a ColorRGB object to LDR using the Uncharted2 tone mapping algorithm.
   * @param color The ColorRGB object to convert.
   * @return The converted ColorRGB object in LDR format.
   */
  ColorRGB convertToLDR(const ColorRGB& color) const override {
    const ColorRGB mapped_color = GetUncharted2ToneMapPartial(color * m_exposure);

    const ColorRGB w(m_white_point);
    const ColorRGB white_scale = ColorRGB(1.0) / GetUncharted2ToneMapPartial(w);
    return mapped_color * white_scale;
  }

private:
  double m_exposure    = 1.0;
  double m_white_point = 1.0;

  static constexpr double A = 0.15;
  static constexpr double B = 0.50;
  static constexpr double C = 0.10;
  static constexpr double D = 0.20;
  static constexpr double E = 0.02;
  static constexpr double F = 0.30;
};

#endif // POSTPROCESSING_TONEMAPPING_UNCHARTED2_HPP