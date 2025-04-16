#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Export/OutputFormatPng.hpp"
#include "stb_image_write.h"

#include <iostream>
#include <string>

void OutputFormatPng::write_image(const std::string& file_path, int width, int height, int channel_count,
                                  const unsigned char* image) const {

  // NOLINTNEXTLINE(clang-analyzer-optin.portability.UnixAPI)
  stbi_write_png((file_path + ".png").c_str(), width, height, channel_count, image, width * channel_count);

  std::cout << "Exporting render to " << file_path << ".png with dimensions" << width << "x" << height << '\n';
}