#include <string>

#include "Export/OutputFormatTga.hpp"
#include "stb_image_write.h"

bool OutputFormatTga::writeImage(const std::string& file_path, int width, int height, int channel_count,
                                  const unsigned char* image) const {
  return (stbi_write_tga(file_path.c_str(), width, height, channel_count, image) != 0);
}
