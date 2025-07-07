/**
 * @file OutputFormatBmp.hpp
 * @brief Header file for the OutputFormatBmp class.
 */
#ifndef EXPORT_OUTPUTFORMATBMP_HPP
#define EXPORT_OUTPUTFORMATBMP_HPP

#include "Export/OutputFormat.hpp"

#include <string>

/**
 * @class OutputFormatBmp
 * @brief Class for exporting images in BMP format.
 */
class OutputFormatBmp : public OutputFormatStrategy {
public:
  bool writeImage(const std::string& file_path, int width, int height, int channel_count,
                   const unsigned char* image) const override;
};

#endif // EXPORT_OUTPUTFORMATBMP_HPP
