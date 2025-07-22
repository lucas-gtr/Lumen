#include "Export/OutputFormatJpeg.hpp"

#include <gtest/gtest.h>
#include <fstream>

TEST(OutputFormatJpegTests, WritesJpegFile) {
  OutputFormatJpeg output_format(95);
  const std::string file_path = "/tmp/test_image.jpeg";
  const int width = 2;
  const int height = 2;
  const int channel_count = 3;
  const unsigned char image[] = {
    255, 0, 0,   0, 255, 0,
    0, 0, 255,   255, 255, 0
  };

  output_format.writeImage(file_path, width, height, channel_count, image);

  std::ifstream file(file_path);
  ASSERT_TRUE(file.good());
  file.close();

  std::remove((file_path).c_str());
}
