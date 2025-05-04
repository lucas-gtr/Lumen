#include "Export/OutputFormatPng.hpp"

#include <gtest/gtest.h>
#include <fstream>

TEST(OutputFormatPngTests, WritesPngFile) {
  OutputFormatPng output_format;
  const std::string file_path = "/tmp/test_image";
  const int width = 2;
  const int height = 2;
  const int channel_count = 3;
  const unsigned char image[] = {
    255, 0, 0,   0, 255, 0,
    0, 0, 255,   255, 255, 0
  };

  output_format.write_image(file_path, width, height, channel_count, image);

  std::ifstream file(file_path + ".png");
  ASSERT_TRUE(file.good());
  file.close();

  std::remove((file_path + ".png").c_str());
}
