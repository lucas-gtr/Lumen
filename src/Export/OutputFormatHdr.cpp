#include <iostream>
#include <string>

#include "Export/OutputFormatHdr.hpp"
#include "stb_image_write.h"

bool OutputFormatHdr::writeImage(const std::string& /*file_path*/, int /*width*/, int /*height*/, int /*channel_count*/,
                                 const unsigned char* /*image*/) const {
  std::cerr << "OutputFormatHdr::write_image is not implemented for unsigned char data.\n";
  std::cerr << "Use write_image_hdr for float data instead.\n";
  return false;
}

bool OutputFormatHdr::WriteImageHdr(const std::string& file_path, int width, int height, int channel_count,
                                    const float* image) {
  // NOLINTNEXTLINE(clang-analyzer-optin.portability.UnixAPI)
  return (stbi_write_hdr(file_path.c_str(), width, height, channel_count, image) != 0);
}
