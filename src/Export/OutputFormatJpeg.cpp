#include <string>

#include "Export/OutputFormatJpeg.hpp"
#include "stb_image_write.h"

OutputFormatJpeg::OutputFormatJpeg(int quality) : m_quality(quality) {}

bool OutputFormatJpeg::writeImage(const std::string& file_path, int width, int height, int channel_count,
                                   const unsigned char* image) const {
  return (stbi_write_jpg(file_path.c_str(), width, height, channel_count, image, m_quality) != 0);
}
