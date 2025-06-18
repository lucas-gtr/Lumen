/**
 * @file OutputFormatHdr.hpp
 * @brief Header file for the OutputFormatHdr class.
 */
#ifndef EXPORT_OUTPUTFORMATHDR_HPP
#define EXPORT_OUTPUTFORMATHDR_HPP

#include "Export/OutputFormat.hpp"

#include <string>

/**
 * @class OutputFormatHdr
 * @brief Class for exporting images in HDR format.
 */
class OutputFormatHdr : public OutputFormatStrategy {
public:
  bool        write_image(const std::string& file_path, int width, int height, int channel_count,
                          const unsigned char* image) const override;
  static bool write_image_hdr(const std::string& file_path, int width, int height, int channel_count,
                              const float* image);
};

#endif // EXPORT_OUTPUTFORMATHDR_HPP
