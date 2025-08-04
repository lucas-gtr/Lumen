#include "Export/OutputFormatHdr.hpp"

#include <gtest/gtest.h>
#include <fstream>

TEST(OutputFormatHdrTests, WriteImage) {
  OutputFormatHdr output_format;
  const std::string file_path = "/tmp/test_image.hdr";
  const int width = 2;
  const int height = 2;
  const int channel_count = 3;
  const unsigned char image[] = {
    255, 0, 0,   0, 255, 0,
    0, 0, 255,   255, 255, 0
  };

  EXPECT_FALSE(output_format.writeImage(file_path, width, height, channel_count, image));

  std::ifstream file(file_path);
  ASSERT_FALSE(file.good());
  file.close();

  std::remove(file_path.c_str());
}

TEST(OutputFormatHdrTests, WritesHdrFile) {
  OutputFormatHdr output_format;
  const std::string file_path = "/tmp/test_image.hdr";
  const int width = 2;
  const int height = 2;
  const int channel_count = 3;
  const float image[] = {
    1.0, 0, 0,   0, 1.0, 0,
    0, 0, 1.0,   1.0, 1.0, 0
  };

  output_format.WriteImageHdr(file_path, width, height, channel_count, image);

  std::ifstream file(file_path);
  ASSERT_TRUE(file.good());
  file.close();

  std::remove((file_path).c_str());
}
