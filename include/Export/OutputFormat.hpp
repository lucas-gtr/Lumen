/**
 * @file OutputFormatStrategy.hpp
 * @brief Header file for the OutputFormatStrategy class.
 */
#ifndef EXPORT_OUTPUTFORMAT_HPP
#define EXPORT_OUTPUTFORMAT_HPP

#include <cstdint>
#include <string>

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

#endif // EXPORT_OUTPUTFORMAT_HPP