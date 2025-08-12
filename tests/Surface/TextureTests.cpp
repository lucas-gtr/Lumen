#include <gtest/gtest.h>

#include "Core/Color.hpp"
#include "Surface/Texture.hpp"

TEST(TextureTest, ConstructFromDouble) {
  Texture texture;
  texture.setValue(0.5);
  EXPECT_NEAR(texture.getValue1d({0.0, 0.0}), 0.5, 1e-9);
}

TEST(TextureTest, ConstructFromVector3d) {
  Texture texture;
  texture.setValue(ColorRGB(0.2, 0.4, 0.6));
  ColorRGB value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.2, 1e-9);
  EXPECT_NEAR(value.g, 0.4, 1e-9);
  EXPECT_NEAR(value.b, 0.6, 1e-9);
}

TEST(TextureTest, ConstructFromVector4d) {
  Texture texture;
  texture.setValue(ColorRGBA(0.1, 0.3, 0.5, 0.7));
  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.1, 1e-9);
  EXPECT_NEAR(value.g, 0.3, 1e-9);
  EXPECT_NEAR(value.b, 0.5, 1e-9);
  EXPECT_NEAR(value.a, 0.7, 1e-9);
}

TEST(TextureTest, SetGrayValue) {
  Texture texture;
  texture.setValue(0.5);
  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(value, 0.5, 1e-9);
}

TEST(TextureTest, SetRGBValue) {
  Texture texture;
  texture.setValue(ColorRGB(0.1, 0.2, 0.3));
  ColorRGB value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.1, 1e-9);
  EXPECT_NEAR(value.g, 0.2, 1e-9);
  EXPECT_NEAR(value.b, 0.3, 1e-9);
}

TEST(TextureTest, SetRGBAValue) {
  Texture texture;
  texture.setValue(ColorRGBA(0.1, 0.2, 0.3, 0.4));
  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.1, 1e-9);
  EXPECT_NEAR(value.g, 0.2, 1e-9);
  EXPECT_NEAR(value.b, 0.3, 1e-9);
  EXPECT_NEAR(value.a, 0.4, 1e-9);
}

TEST(TextureTest, SetBorderColor) {
  ColorRGBA borderColor_4d(1.0, 0.5, 0.5, 1.0);
  Texture texture_4d;
  texture_4d.setValue(ColorRGBA(0.0, 0.0, 0.0, 0.0));
  texture_4d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_4d.setBorderColor(borderColor_4d);
  ColorRGBA value = texture_4d.getValue4d({-1.0, -1.0});

  EXPECT_EQ(ColorRGBA(texture_4d.getBorderColor()), borderColor_4d);

  EXPECT_EQ(value.r, 1.0);
  EXPECT_EQ(value.g, 0.5);
  EXPECT_EQ(value.b, 0.5);
  EXPECT_EQ(value.a, 1.0);

  ColorRGB borderColor_3d(1.0, 0.5, 0.5);
  Texture texture_3d;
  texture_3d.setValue(ColorRGB(0.0, 0.0, 0.0));
  texture_3d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_3d.setBorderColor(borderColor_3d);
  ColorRGB rgb_value = texture_3d.getValue3d({-1.0, -1.0});
  EXPECT_EQ(rgb_value.r, 1.0);
  EXPECT_EQ(rgb_value.g, 0.5);
  EXPECT_EQ(rgb_value.b, 0.5);


  double borderColor_1d(0.67);
  Texture texture_1d;
  texture_1d.setValue(0.0);
  texture_1d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_1d.setBorderColor(borderColor_1d);
  double gray_value = texture_1d.getValue1d({-1.0, -1.0});
  EXPECT_NEAR(gray_value, 0.67, 1e-9);

}

TEST(TextureTest, GetValueFromGrayScale) {
  Texture texture;
  texture.setValue(0.5);
  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, 0.5, 1e-9);

  ColorRGB rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value.r, 0.5, 1e-9);
  EXPECT_NEAR(rgb_value.g, 0.5, 1e-9);
  EXPECT_NEAR(rgb_value.b, 0.5, 1e-9);

  ColorRGBA rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value.r, 0.5, 1e-9);
  EXPECT_NEAR(rgba_value.g, 0.5, 1e-9);
  EXPECT_NEAR(rgba_value.b, 0.5, 1e-9);
  EXPECT_NEAR(rgba_value.a, 1.0, 1e-9);
}

TEST(TextureTest, GetValueFromRGB) {
  Texture texture;
  texture.setValue(ColorRGB(0.2, 0.4, 0.6));
  ColorRGB rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value.r, 0.2, 1e-9);
  EXPECT_NEAR(rgb_value.g, 0.4, 1e-9);
  EXPECT_NEAR(rgb_value.b, 0.6, 1e-9);

  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, rgb_value.grayscale(), 1e-9);

  ColorRGBA rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value.r, 0.2, 1e-9);
  EXPECT_NEAR(rgba_value.g, 0.4, 1e-9);
  EXPECT_NEAR(rgba_value.b, 0.6, 1e-9);
  EXPECT_NEAR(rgba_value.a, 1.0, 1e-9);
}

TEST(TextureTest, GetValueFromRGBA) {
  Texture texture;
  texture.setValue(ColorRGBA(0.1, 0.3, 0.5, 0.7));
  ColorRGBA rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value.r, 0.1, 1e-9);
  EXPECT_NEAR(rgba_value.g, 0.3, 1e-9);
  EXPECT_NEAR(rgba_value.b, 0.5, 1e-9);
  EXPECT_NEAR(rgba_value.a, 0.7, 1e-9);

  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, rgba_value.grayscale(), 1e-9);

  ColorRGB rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value.r, 0.1, 1e-9);
  EXPECT_NEAR(rgb_value.g, 0.3, 1e-9);
  EXPECT_NEAR(rgb_value.b, 0.5, 1e-9);
}

TEST(TextureTest, GetValueNearest) {
  Texture texture;
  texture.setValue(ColorRGB(0.2, 0.4, 0.6));
  texture.setFilteringMode(TextureSampling::TextureFiltering::NEAREST);
  
  ColorRGB value = texture.getValue3d({0.5, 0.5});
  EXPECT_NEAR(value.r, 0.2, 1e-9);
  EXPECT_NEAR(value.g, 0.4, 1e-9);
  EXPECT_NEAR(value.b, 0.6, 1e-9);
}

TEST(TextureTest, ColorSpaceDefaultSRGB) {
  double gray = 0.5;
  Texture texture;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::S_RGB);
  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(value, 0.5, 1e-6);
}

TEST(TextureTest, ColorSpaceConversionSRGBToLinear) {
  double gray = 0.5;
  Texture texture;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::LINEAR);
  double value = texture.getValue1d({0.0, 0.0});
  convertToLinearSpace(gray);
  EXPECT_NEAR(value, gray, 1e-9);
}

TEST(TextureTest, ColorSpaceConversionLinearToSRGB) {
  Texture texture;
  texture.setValue(0.0);
  texture.setColorSpace(ColorSpace::LINEAR);
  double gray = 0.5;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::S_RGB);

  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(value, gray, 1e-9);
}

TEST(TextureTest, GetWrappingMode) {
  Texture texture;
  EXPECT_EQ(texture.getWrappingMode(), TextureSampling::TextureWrapping::MIRRORED_REPEAT);

  texture.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  EXPECT_EQ(texture.getWrappingMode(), TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
}

TEST(TextureTest, GetFilteringMode) {
  Texture texture;
  EXPECT_EQ(texture.getFilteringMode(), TextureSampling::TextureFiltering::BILINEAR);

  texture.setFilteringMode(TextureSampling::TextureFiltering::NEAREST);
  EXPECT_EQ(texture.getFilteringMode(), TextureSampling::TextureFiltering::NEAREST);
}

TEST(TextureTest, GetColorSpace) {
  Texture texture;
  EXPECT_EQ(texture.getColorSpace(), ColorSpace::S_RGB);

  texture.setColorSpace(ColorSpace::LINEAR);
  EXPECT_EQ(texture.getColorSpace(), ColorSpace::LINEAR);
}

TEST(TextureTest, GetFlippedVertically) {
  Texture texture;
  EXPECT_FALSE(texture.isFlippedVertically());

  texture.setFlippedVertically(true);
  EXPECT_TRUE(texture.isFlippedVertically());
}

TEST(TextureTest, GetTextureType) {
  Texture texture;
  texture.setTextureType(TextureType::IMAGE_TEXTURE);
  EXPECT_EQ(texture.getTextureType(), TextureType::IMAGE_TEXTURE);
  EXPECT_EQ(texture.getValue3d({0.0, 0.0}), ColorRGB(1.0, 0.0, 1.0));

  texture.setTextureType(TextureType::COLOR_TEXTURE);
  EXPECT_EQ(texture.getTextureType(), TextureType::COLOR_TEXTURE);
  EXPECT_EQ(texture.getValue3d({0.0, 0.0}), ColorRGB(1.0, 1.0, 1.0));
}

TEST(TextureTest, GetTexturePath) {
  Texture texture;
  EXPECT_TRUE(texture.getTexturePath().empty());

  const std::string path = "test_texture.png";
  texture.loadFromFile(path.c_str());
  EXPECT_EQ(texture.getTexturePath(), path);
}

TEST(TextureTest, GetProperties) {
  Texture texture;
  texture.setValue(ColorRGB(0.5, 0.5, 0.5));
  
  const auto& properties = texture.getProperties();
  EXPECT_EQ(properties.width, 1);
  EXPECT_EQ(properties.height, 1);
  EXPECT_EQ(properties.channels, 3);

  const auto& preview_properties = texture.getPreviewProperties();
  EXPECT_EQ(preview_properties.width, 1);
  EXPECT_EQ(preview_properties.height, 1);
  EXPECT_EQ(preview_properties.channels, 3);
  
  texture.setValue(ColorRGBA(0.5, 0.5, 0.5, 1.0));
  
  const auto& rgba_properties = texture.getProperties();
  EXPECT_EQ(rgba_properties.channels, 4);

  const auto& rgba_preview_properties = texture.getPreviewProperties();
  EXPECT_EQ(rgba_preview_properties.channels, 4);
}

TEST(TextureTest, GetImageData) {
  Texture texture;
  texture.setValue(ColorRGBA(0.3, 0.6, 0.9, 1.0));

  const double* image_data = texture.getImageData();
  EXPECT_NEAR(image_data[0], 0.3, 1e-9);
  EXPECT_NEAR(image_data[1], 0.6, 1e-9);
  EXPECT_NEAR(image_data[2], 0.9, 1e-9);
  EXPECT_EQ(image_data[3], 1.0);

  const unsigned char* preview_data = texture.getPreviewData();
  EXPECT_EQ(preview_data[0], static_cast<unsigned char>(0.3 * NORMALIZED_TO_COLOR8));
  EXPECT_EQ(preview_data[1], static_cast<unsigned char>(0.6 * NORMALIZED_TO_COLOR8));
  EXPECT_EQ(preview_data[2], static_cast<unsigned char>(0.9 * NORMALIZED_TO_COLOR8));
  EXPECT_EQ(preview_data[3], 255);
}

TEST(TextureTest, FlipVertically) {
  ImageProperties properties{3, 3, 1};
  std::vector<double> image_data = {
      1.0, 2.0, 3.0,
      4.0, 5.0, 6.0,
      7.0, 8.0, 9.0
  };

  Texture texture;
  texture.generateTexture(properties, image_data);
  texture.setFlippedVertically(true);

  const double* flipped_data = texture.getImageData();
  EXPECT_EQ(flipped_data[0], 7.0);
  EXPECT_EQ(flipped_data[1], 8.0);
  EXPECT_EQ(flipped_data[2], 9.0);
  EXPECT_EQ(flipped_data[3], 4.0);
  EXPECT_EQ(flipped_data[4], 5.0);
  EXPECT_EQ(flipped_data[5], 6.0);
  EXPECT_EQ(flipped_data[6], 1.0);
  EXPECT_EQ(flipped_data[7], 2.0);
  EXPECT_EQ(flipped_data[8], 3.0);
}

TEST(TextureTest, PreviewDataWidthFit) {
  ImageProperties properties{1000, 500, 3};
  std::vector<double> image_data(properties.bufferSize(), 0.5);

  Texture texture;
  texture.generateTexture(properties, image_data);
  texture.generatePreviewData();

  const auto& preview_properties = texture.getPreviewProperties();
  EXPECT_EQ(preview_properties.width, 256);
  EXPECT_EQ(preview_properties.height, 128);
  EXPECT_EQ(preview_properties.channels, 3);
}

TEST(TextureTest, PreviewDataHeightFit) {
  ImageProperties properties{500, 1000, 3};
  std::vector<double> image_data(properties.bufferSize(), 0.5);

  Texture texture;
  texture.generateTexture(properties, image_data);
  texture.generatePreviewData();

  const auto& preview_properties = texture.getPreviewProperties();
  EXPECT_EQ(preview_properties.width, 128);
  EXPECT_EQ(preview_properties.height, 256);
  EXPECT_EQ(preview_properties.channels, 3);
}

TEST(TextureTest, TextureDataObserver) {
  Texture texture;
  bool notified = false;

  texture.getTextureDataObserver().add([&notified]() {
    notified = true;
  });

  texture.setValue(0.5);
  EXPECT_TRUE(notified);

  notified = false;

  texture.setValue(ColorRGB(0.2, 0.4, 0.6));
  EXPECT_TRUE(notified);

  notified = false;
  texture.setValue(ColorRGBA(0.1, 0.3, 0.5, 0.7));
  EXPECT_TRUE(notified);

  notified = false;
  texture.setFlippedVertically(true);
  EXPECT_TRUE(notified);

  notified = false;
  texture.setTextureType(TextureType::IMAGE_TEXTURE);
  EXPECT_TRUE(notified);

  texture.loadFromFile("test_texture.png");
  EXPECT_TRUE(notified);
}

TEST(TextureTest, TextureParametersObserver) {
  Texture texture;
  bool notified = false;

  texture.getTextureParametersObserver().add([&notified]() {
    notified = true;
  });

  texture.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  EXPECT_TRUE(notified);

  notified = false;
  texture.setFilteringMode(TextureSampling::TextureFiltering::NEAREST);
  EXPECT_TRUE(notified);

  notified = false;
  texture.setColorSpace(ColorSpace::LINEAR);
  EXPECT_TRUE(notified);
}