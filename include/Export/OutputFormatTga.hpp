/**
 * @file OutputFormatTga.hpp
 * @brief Header file for the OutputFormatTga class.
 */
#ifndef EXPORT_OUTPUTFORMATTGA_HPP
#define EXPORT_OUTPUTFORMATTGA_HPP

#include "Export/OutputFormat.hpp"

#include <string>

/**
 * @class OutputFormatTga
 * @brief Class for exporting images in TGA format.
 */
class OutputFormatTga : public OutputFormatStrategy {
public:
  bool writeImage(const std::string& file_path, int width, int height, int channel_count,
                   const unsigned char* image) const override;
};

#endif // EXPORT_OUTPUTFORMATTGA_HPP
