/**
 * @file OutputFormat.hpp
 * @brief Header file for the OutputFormatStrategy class.
 */
#ifndef EXPORT_OUTPUTFORMAT_HPP
#define EXPORT_OUTPUTFORMAT_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class OutputFormat : uint8_t { PNG, JPEG, BMP, TGA, HDR };

/**
 * @class OutputFormatStrategy
 * @brief Abstract base class for different image output formats.
 *
 * This class defines the interface for writing images to files in various formats.
 * Derived classes should implement the write_image method for specific formats.
 */
class OutputFormatStrategy {
public:
  OutputFormatStrategy()                                       = default;
  OutputFormatStrategy(const OutputFormatStrategy&)            = delete;
  OutputFormatStrategy& operator=(const OutputFormatStrategy&) = delete;
  OutputFormatStrategy(OutputFormatStrategy&&)                 = delete;
  OutputFormatStrategy& operator=(OutputFormatStrategy&&)      = delete;

  /**
   * @brief Pure virtual function to write an image to a file.
   * @param file_path The path to the file where the image will be saved.
   * @param width The width of the image.
   * @param height The height of the image.
   * @param channel_count The number of color channels in the image.
   * @param image Pointer to the image data (unsigned char array).
   */
  virtual bool writeImage(const std::string& file_path, int width, int height, int channel_count,
                          const unsigned char* image) const = 0;

  virtual ~OutputFormatStrategy() = default; ///< Default destructor.
};

/**
 * @brief Defines a mapping from string representations to OutputFormat enum values.
 * @return A constant reference to an unordered map where keys are format names and values are OutputFormat enum values.
 */
inline const std::unordered_map<std::string, OutputFormat>& stringToOutputFormatMap() {
  static const std::unordered_map<std::string, OutputFormat> map = {{"HDR", OutputFormat::HDR},
                                                                    {"BMP", OutputFormat::BMP},
                                                                    {"TGA", OutputFormat::TGA},
                                                                    {"JPEG", OutputFormat::JPEG},
                                                                    {"PNG", OutputFormat::PNG}};
  return map;
}

/**
 * @brief Converts an OutputFormat enum to its string representation.
 * @param format The OutputFormat enum value.
 * @return The string representation of the output format.
 */
inline std::string outputFormatToString(OutputFormat format) {
  for(const auto& [name, value] : stringToOutputFormatMap()) {
    if(value == format) {
      return name;
    }
  }
  return "PNG";
}

/**
 * @brief Converts a string to its corresponding OutputFormat enum value.
 * @param str The string representation of the output format.
 * @return The corresponding OutputFormat enum value.
 */
inline OutputFormat stringToOutputFormat(const std::string& str) {
  const auto& map = stringToOutputFormatMap();
  auto        it  = map.find(str);
  return (it != map.end()) ? it->second : OutputFormat::PNG; // Default to PNG if not found
}

/**
 * @brief Returns a list of all output format names (for UI display).
 * @return A vector of strings containing the names of all available output formats.
 */
inline std::vector<std::string> getOutputFormatNames() {
  std::vector<std::string> format_names;
  const auto&              map = stringToOutputFormatMap();
  format_names.reserve(map.size());
  for(const auto& pair : map) {
    format_names.push_back(pair.first);
  }
  return format_names;
}

#endif // EXPORT_OUTPUTFORMAT_HPP