#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <memory>

#include "Surface/TextureLoader.hpp"
#include "Surface/Texture.hpp"

#include "stb_image_write.h"

class TextureLoaderTest : public ::testing::Test {
protected:
  std::string testImageFilename = "test_texture.png";

  void SetUp() override {
    const int width = 2;
    const int height = 2;
    const int channels = 3;

    unsigned char imageData[width * height * channels] = {
      255, 0, 0,     0, 255, 0,
      0, 0, 255,     255, 255, 0
    };

    stbi_write_png(testImageFilename.c_str(), width, height, channels, imageData, width * channels);
  }

  void TearDown() override {
    std::remove(testImageFilename.c_str());
  }
};
