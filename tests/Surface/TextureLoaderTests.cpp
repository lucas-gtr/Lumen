#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <memory>

#include "Surface/TextureLoader.hpp"
#include "Surface/Texture.hpp"

#include "stb_image_write.h"

TEST(TextureLoaderTest, LoadValidImage) {
  std::string testImageFilename = "test_texture.png";

  const int width = 2;
  const int height = 2;
  const int channels = 3;

  unsigned char imageData[width * height * channels] = {
    255, 0, 0,     0, 255, 0,
    0, 0, 255,     255, 255, 0
  };

  stbi_write_png(testImageFilename.c_str(), width, height, channels, imageData, width * channels);

  std::vector<double> vecImageData;
  ImageProperties textureProperties;
  TextureLoader::load(testImageFilename.c_str(), vecImageData, textureProperties);

  EXPECT_EQ(textureProperties.width, 2);
  EXPECT_EQ(textureProperties.height, 2);
  EXPECT_EQ(textureProperties.channels, 3);

  EXPECT_NEAR(vecImageData[0], 1.0, 1e-9);
  EXPECT_NEAR(vecImageData[1], 0.0, 1e-9);
  EXPECT_NEAR(vecImageData[2], 0.0, 1e-9);

  EXPECT_NEAR(vecImageData[3], 0.0, 1e-9);
  EXPECT_NEAR(vecImageData[4], 1.0, 1e-9);
  EXPECT_NEAR(vecImageData[5], 0.0, 1e-9);

  std::remove(testImageFilename.c_str());
}
TEST(TextureLoaderTest, LoadValidHDRImage) {
  std::string testHDRImageFilename = "test_texture.hdr";

  const int width = 2;
  const int height = 2;
  const int channels = 3;

  float imageData[width * height * channels] = {
    1.0f, 2.0f, 3.0f,   0.0f, 0.3f, 0.6f,
    0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f
  };

  stbi_write_hdr(testHDRImageFilename.c_str(), width, height, channels, imageData);

  std::vector<double> vecImageData;
  ImageProperties textureProperties;
  TextureLoader::load(testHDRImageFilename.c_str(), vecImageData, textureProperties);

  EXPECT_EQ(textureProperties.width, 2);
  EXPECT_EQ(textureProperties.height, 2);
  EXPECT_EQ(textureProperties.channels, 3);

  EXPECT_NEAR(vecImageData[0], 1.0, 1e-6);
  EXPECT_NEAR(vecImageData[1], 2.0, 1e-6);
  EXPECT_NEAR(vecImageData[2], 3.0, 1e-6);

  EXPECT_NEAR(vecImageData[3], 0.0, 1e-6);
  EXPECT_NEAR(vecImageData[4], 0.3, 1e-2);
  EXPECT_NEAR(vecImageData[5], 0.6, 1e-2);

  std::remove(testHDRImageFilename.c_str());
}

TEST(TextureLoaderTest, LoadInvalidImage) {
  std::string invalidImageFilename = "invalid_texture.png";

  std::vector<double> vecImageData;
  ImageProperties textureProperties;

  TextureLoader::load(invalidImageFilename.c_str(), vecImageData, textureProperties);

  EXPECT_EQ(textureProperties.width, 1);
  EXPECT_EQ(textureProperties.height, 1);
  EXPECT_EQ(textureProperties.channels, 3);
  
  EXPECT_EQ(vecImageData.size(), 3);
  EXPECT_DOUBLE_EQ(vecImageData[0], 1.0);
  EXPECT_DOUBLE_EQ(vecImageData[1], 0.0);
  EXPECT_DOUBLE_EQ(vecImageData[2], 1.0);
}

TEST(TextureLoaderTest, LoadNoExtensionImage) {
  std::string noExtensionFilename = "no_extension_file";

  std::vector<double> vecImageData;
  ImageProperties textureProperties;

  TextureLoader::load(noExtensionFilename.c_str(), vecImageData, textureProperties);

  EXPECT_EQ(textureProperties.width, 1);
  EXPECT_EQ(textureProperties.height, 1);
  EXPECT_EQ(textureProperties.channels, 3);
  
  EXPECT_EQ(vecImageData.size(), 3);
  EXPECT_DOUBLE_EQ(vecImageData[0], 1.0);
  EXPECT_DOUBLE_EQ(vecImageData[1], 0.0);
  EXPECT_DOUBLE_EQ(vecImageData[2], 1.0);
}

TEST(TextureLoaderTest, LoadInvalidHdrImage) {
  std::string invalidImageFilename = "invalid_texture.hdr";

  std::vector<double> vecImageData;
  ImageProperties textureProperties;

  TextureLoader::load(invalidImageFilename.c_str(), vecImageData, textureProperties);

  EXPECT_EQ(textureProperties.width, 1);
  EXPECT_EQ(textureProperties.height, 1);
  EXPECT_EQ(textureProperties.channels, 3);
  
  EXPECT_EQ(vecImageData.size(), 3);
  EXPECT_DOUBLE_EQ(vecImageData[0], 1.0);
  EXPECT_DOUBLE_EQ(vecImageData[1], 0.0);
  EXPECT_DOUBLE_EQ(vecImageData[2], 1.0);
}
