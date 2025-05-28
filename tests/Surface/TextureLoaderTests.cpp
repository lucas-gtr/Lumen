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

TEST_F(TextureLoaderTest, LoadValidImage) {
  std::shared_ptr<Texture> texture = TextureLoader::load(testImageFilename.c_str());
  ASSERT_NE(texture, nullptr);

  const ImageProperties& props = texture->getProperties();
  EXPECT_EQ(props.width, 2);
  EXPECT_EQ(props.height, 2);
  EXPECT_EQ(props.channels, 3);

  texture->setFilteringMode(TextureSampling::TextureFiltering::NEAREST);

  const ColorRGBA& color = texture->getValue4d({0, 1});
  EXPECT_EQ(color, ColorRGBA(0.0, 0.0, 1.0, 1.0));
}

TEST(TextureLoaderErrorTest, LoadNonexistentImage) {
  std::shared_ptr<Texture> texture = TextureLoader::load("does_not_exist.png");
  ASSERT_NE(texture, nullptr);

  const ColorRGBA& color = texture->getValue4d({0, 0});
  EXPECT_EQ(color, ColorRGBA(1.0, 0.0, 1.0, 1.0));
}
