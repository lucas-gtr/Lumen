/**
 * @file OutputFormatJpeg.hpp
 * @brief Header file for the OutputFormatJpeg class.
 */
#ifndef EXPORT_OUTPUTFORMATJPEG_HPP
#define EXPORT_OUTPUTFORMATJPEG_HPP

#include "Export/OutputFormat.hpp"

#include <stdexcept>
#include <string>

/**
 * @class OutputFormatJpeg
 * @brief Class for exporting images in JPEG format.
 */
class OutputFormatJpeg : public OutputFormatStrategy {
private:
  int m_quality;

public:
  /**
   * @brief Constructor for OutputFormatJpeg.
   * @param quality JPEG quality level (0-100).
   */
  explicit OutputFormatJpeg(int quality);

  bool writeImage(const std::string& file_path, int width, int height, int channel_count,
                  const unsigned char* image) const override;
};

#endif // EXPORT_OUTPUTFORMATJPEG_HPP
