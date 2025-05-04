/**
 * @file OutputFormatPng.hpp
 * @brief Header file for the OutputFormatPng class.
 */
#ifndef EXPORT_OUTPUTFORMATPNG_HPP
#define EXPORT_OUTPUTFORMATPNG_HPP

#include "Export/OutputFormat.hpp"

#include <string>

/**
 * @class OutputFormatPng
 * @brief Class for exporting images in PNG format.
 */
class OutputFormatPng : public OutputFormat {
public:
  /**
   * @brief Override the write_image method to save an image in PNG format.
   * @param file_path The path to the file where the image will be saved.
   * @param width The width of the image.
   * @param height The height of the image.
   * @param channel_count The number of color channels in the image.
   * @param image Pointer to the image data (unsigned char array).
   */
  void write_image(const std::string& file_path, int width, int height, int channel_count,
                   const unsigned char* image) const override;
};

#endif // EXPORT_OUTPUTFORMATPNG_HPP