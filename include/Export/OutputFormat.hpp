/**
 * @file OutputFormat.hpp
 * @brief Header file for the OutputFormat class.
 */
#ifndef EXPORT_OUTPUTFORMAT_HPP
#define EXPORT_OUTPUTFORMAT_HPP

#include <string>

class OutputFormat {

public:
  OutputFormat()                               = default; ///< Default constructor.
  OutputFormat(const OutputFormat&)            = default; ///< Default copy constructor.
  OutputFormat& operator=(const OutputFormat&) = default; ///< Default copy assignment operator.
  OutputFormat(OutputFormat&&)                 = default; ///< Default move constructor.
  OutputFormat& operator=(OutputFormat&&)      = default; ///< Default move assignment operator.

  /**
   * @brief Pure virtual function to write an image to a file.
   * @param file_path The path to the file where the image will be saved.
   * @param width The width of the image.
   * @param height The height of the image.
   * @param channel_count The number of color channels in the image.
   * @param image Pointer to the image data (unsigned char array).
   */
  virtual void write_image(const std::string& file_path, int width, int height, int channel_count,
                           const unsigned char* image) const = 0;

  virtual ~OutputFormat() = default; ///< Default destructor.
};

#endif // EXPORT_OUTPUTFORMAT_HPP