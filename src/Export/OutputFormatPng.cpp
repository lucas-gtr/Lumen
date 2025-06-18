#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Export/OutputFormatPng.hpp"
#include "stb_image_write.h"

bool OutputFormatPng::write_image(const std::string& file_path, int width, int height, int channel_count,
                                  const unsigned char* image) const {

  // NOLINTNEXTLINE(clang-analyzer-optin.portability.UnixAPI)
  return (stbi_write_png(file_path.c_str(), width, height, channel_count, image, width * channel_count) != 0);
}