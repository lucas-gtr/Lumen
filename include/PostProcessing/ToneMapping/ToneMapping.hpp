/**
 * @file ToneMapping.hpp
 * @brief Header file for the ToneMappingStrategy class.
 */
#ifndef POSTPROCESSING_TONEMAPPING_TONEMAPPING_HPP
#define POSTPROCESSING_TONEMAPPING_TONEMAPPING_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Core/Color.hpp"

/**
 * @brief Enumeration for different tone mapping strategies.
 */
enum class ToneMapping : std::uint8_t { NONE, LUMINANCE_REINHARD, WHITE_POINT_REINHARD, EXPOSURE, ACES, UNCHARTED2 };

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

  virtual double convertToLDR(double value) const = 0;

  virtual ColorRGB convertToLDR(const ColorRGB& color) const = 0;

  void apply(const double* framebuffer, unsigned char* output, int pixel_count, int channels) const {
    for(int base_index = 0; base_index < pixel_count * channels; base_index += channels) {
      switch(channels) {
      case 1:
        output[base_index] = static_cast<unsigned char>(convertToLDR(framebuffer[base_index]) * NORMALIZED_TO_COLOR8);
        break;
      case 3:
      case 4: {
        const ColorRGB pixel_color = {framebuffer[base_index], framebuffer[base_index + 1],
                                      framebuffer[base_index + 2]};
        ColorRGB       ldr_color   = convertToLDR(pixel_color);
        ldr_color.clamp(0.0, 1.0);

        output[base_index]     = static_cast<unsigned char>(ldr_color.r * NORMALIZED_TO_COLOR8);
        output[base_index + 1] = static_cast<unsigned char>(ldr_color.g * NORMALIZED_TO_COLOR8);
        output[base_index + 2] = static_cast<unsigned char>(ldr_color.b * NORMALIZED_TO_COLOR8);
        break;
      }
      default:
        throw std::runtime_error("Unsupported number of channels for tone mapping: " + std::to_string(channels));
      }

      if(channels == 4) {
        const double alpha     = std::clamp(framebuffer[base_index + 3], 0.0, 1.0);
        output[base_index + 3] = static_cast<unsigned char>(alpha * NORMALIZED_TO_COLOR8);
      }
    }
  }

  /**
   * @brief Virtual destructor for the ToneMappingStrategy class.
   */
  virtual ~ToneMappingStrategy() = default;
};

/**
 * @brief Returns a mapping from string representations to ToneMapping enum values.
 * @return A constant reference to an unordered map where keys are tone mapping names and values are
 */
inline const std::unordered_map<std::string, ToneMapping>& stringToToneMappingMap() {
  static const std::unordered_map<std::string, ToneMapping> map = {
      {"Luminance Reinhard", ToneMapping::LUMINANCE_REINHARD},
      {"White Point Reinhard", ToneMapping::WHITE_POINT_REINHARD},
      {"Exposure", ToneMapping::EXPOSURE},
      {"Uncharted2", ToneMapping::UNCHARTED2},
      {"ACES", ToneMapping::ACES},
      {"None", ToneMapping::NONE}};
  return map;
}

/**
 * @brief Converts a ToneMapping enum to its string representation.
 * @param mapping The ToneMapping enum value.
 * @return The string representation of the tone mapping.
 */
inline std::string toneMappingToString(ToneMapping mapping) {
  for(const auto& [name, value] : stringToToneMappingMap()) {
    if(value == mapping) {
      return name;
    }
  }
  return "None";
}

/**
 * @brief Converts a string to its corresponding ToneMapping enum value.
 * @param str The string representation of the tone mapping.
 * @return The corresponding ToneMapping enum value.
 */
inline ToneMapping stringToToneMapping(const std::string& str) {
  const auto& map = stringToToneMappingMap();
  auto        it  = map.find(str);
  return (it != map.end()) ? it->second : ToneMapping::NONE;
}

/**
 * @brief Returns a list of all tone mapping names (for UI display).
 * @return A vector of strings containing all available tone mapping names.
 */
inline std::vector<std::string> availableToneMappingNames() {
  std::vector<std::string> names;
  for(const auto& [name, _] : stringToToneMappingMap()) {
    names.push_back(name);
  }
  return names;
}

#endif // POSTPROCESSING_TONEMAPPING_TONEMAPPING_HPP