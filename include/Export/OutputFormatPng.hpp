/**
 * @file OutputFormatPng.hpp
 * @brief Header file for the OutputFormatPng class.
 */
#ifndef EXPORT_OUTPUTFORMATPNG_HPP
#define EXPORT_OUTPUTFORMATPNG_HPP

#include "Export/OutputFormat.hpp"
#include <string>

class OutputFormatPng : public OutputFormat {
public:
  void write_image(const std::string& file_path, int width, int height, int channel_count,
                   const unsigned char* image) const override;
};

#endif // EXPORT_OUTPUTFORMATPNG_HPP