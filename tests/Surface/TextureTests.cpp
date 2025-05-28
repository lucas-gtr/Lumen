#include <gtest/gtest.h>
#include <Eigen/Dense>

#include "Core/ColorUtils.hpp"
#include "Surface/Texture.hpp"

TEST(TextureTest, ConstructFromDouble) {
  Texture texture(0.5);
  EXPECT_NEAR(texture.getValue1d({0.0, 0.0}), 0.5, 1e-9);
}

TEST(TextureTest, ConstructFromVector3d) {
  Texture texture(ColorRGB(0.2, 0.4, 0.6));
  ColorRGB value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.2, 1e-9);
  EXPECT_NEAR(value.g, 0.4, 1e-9);
  EXPECT_NEAR(value.b, 0.6, 1e-9);
}

TEST(TextureTest, ConstructFromVector4d) {
  Texture texture(ColorRGBA(0.1, 0.3, 0.5, 0.7));
  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.1, 1e-9);
  EXPECT_NEAR(value.g, 0.3, 1e-9);
  EXPECT_NEAR(value.b, 0.5, 1e-9);
  EXPECT_NEAR(value.a, 0.7, 1e-9);
}

TEST(TextureTest, ConstructorFromImageData) {
  unsigned char imageData[4] = {255, 0, 255, 128};
  ImageProperties properties = {1, 1, 4};
  Texture texture(imageData, properties);
  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_EQ(value.r, 1.0);
  EXPECT_EQ(value.g, 0.0);
  EXPECT_EQ(value.b, 1.0);
  EXPECT_NEAR(value.a, 0.5, 1e-2);
}

TEST(TextureTest, ConstructorFromDoubleArray) {
  double imageData[4] = {1.0, 0.0, 0.0, 1.0};
  ImageProperties properties = {1, 1, 4};
  Texture texture(imageData, properties);
  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_EQ(value.r, 1.0);
  EXPECT_EQ(value.g, 0.0);
  EXPECT_EQ(value.b, 0.0);
  EXPECT_EQ(value.a, 1.0);
}

TEST(TextureTest, CopyConstructor) {
  Texture original(ColorRGB(0.6, 0.7, 0.8));
  Texture copy = original;
  ColorRGB value = copy.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.6, 1e-9);
  EXPECT_NEAR(value.g, 0.7, 1e-9);
  EXPECT_NEAR(value.b, 0.8, 1e-9);
}

TEST(TextureTest, MoveConstructor) {
  Texture original(ColorRGB(0.6, 0.7, 0.8));
  Texture moved = std::move(original);
  ColorRGB value = moved.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.6, 1e-9);
  EXPECT_NEAR(value.g, 0.7, 1e-9);
  EXPECT_NEAR(value.b, 0.8, 1e-9);
}

TEST(TextureTest, CopyAssignment) {
  Texture original(ColorRGB(0.6, 0.7, 0.8));
  Texture copy(0.0);
  copy = original;
  
  original.setValue(ColorRGB(0.1, 0.2, 0.3));
  ColorRGB original_value = original.getValue3d({0.0, 0.0});
  EXPECT_NEAR(original_value.r, 0.1, 1e-9);
  EXPECT_NEAR(original_value.g, 0.2, 1e-9);
  EXPECT_NEAR(original_value.b, 0.3, 1e-9);
  ColorRGB copy_value = copy.getValue3d({0.0, 0.0});
  EXPECT_NEAR(copy_value.r, 0.6, 1e-9);
  EXPECT_NEAR(copy_value.g, 0.7, 1e-9);
  EXPECT_NEAR(copy_value.b, 0.8, 1e-9);
}

TEST(TextureTest, MoveAssignment) {
  Texture original(ColorRGB(0.6, 0.7, 0.8));
  Texture moved(0.0);
  moved = std::move(original);

  ColorRGB moved_value = moved.getValue3d({0.0, 0.0});
  EXPECT_NEAR(moved_value.r, 0.6, 1e-9);
  EXPECT_NEAR(moved_value.g, 0.7, 1e-9);
  EXPECT_NEAR(moved_value.b, 0.8, 1e-9);

  EXPECT_NE(&original, &moved);
}

TEST(TextureTest, SetGrayValue) {
  Texture texture(0.0);
  texture.setValue(0.5);
  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(value, 0.5, 1e-9);
}

TEST(TextureTest, SetRGBValue) {
  Texture texture(ColorRGB(0.0, 0.0, 0.0));
  texture.setValue(ColorRGB(0.1, 0.2, 0.3));
  ColorRGB value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.1, 1e-9);
  EXPECT_NEAR(value.g, 0.2, 1e-9);
  EXPECT_NEAR(value.b, 0.3, 1e-9);
}

TEST(TextureTest, SetRGBAValue) {
  Texture texture(ColorRGBA(0.0, 0.0, 0.0, 0.0));
  texture.setValue(ColorRGBA(0.1, 0.2, 0.3, 0.4));
  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(value.r, 0.1, 1e-9);
  EXPECT_NEAR(value.g, 0.2, 1e-9);
  EXPECT_NEAR(value.b, 0.3, 1e-9);
  EXPECT_NEAR(value.a, 0.4, 1e-9);
}

TEST(TextureTest, SetImageData) {
  Texture texture(1.0);
  
  double imageData[4] = {1.0, 0.0, 0.0, 1.0};
  ImageProperties properties = {1, 1, 4};
  texture.setImageData(imageData, properties);

  ColorRGBA value = texture.getValue4d({0.0, 0.0});
  EXPECT_EQ(value.r, 1.0);
  EXPECT_EQ(value.g, 0.0);
  EXPECT_EQ(value.b, 0.0);
  EXPECT_EQ(value.a, 1.0);
}

TEST(TextureTest, SetBorderColor) {
  ColorRGBA borderColor_4d(1.0, 0.5, 0.5, 1.0);
  Texture texture_4d(ColorRGBA(0.0, 0.0, 0.0, 0.0));
  texture_4d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_4d.setBorderColor(borderColor_4d);
  ColorRGBA value = texture_4d.getValue4d({-1.0, -1.0});
  EXPECT_EQ(value.r, 1.0);
  EXPECT_EQ(value.g, 0.5);
  EXPECT_EQ(value.b, 0.5);
  EXPECT_EQ(value.a, 1.0);

  ColorRGB borderColor_3d(1.0, 0.5, 0.5);
  Texture texture_3d(ColorRGB(0.0, 0.0, 0.0));
  texture_3d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_3d.setBorderColor(borderColor_3d);
  ColorRGB rgb_value = texture_3d.getValue3d({-1.0, -1.0});
  EXPECT_EQ(rgb_value.r, 1.0);
  EXPECT_EQ(rgb_value.g, 0.5);
  EXPECT_EQ(rgb_value.b, 0.5);

  double borderColor_1d(0.67);
  Texture texture_1d(0.0);
  texture_1d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_1d.setBorderColor(borderColor_1d);
  double gray_value = texture_1d.getValue1d({-1.0, -1.0});
  EXPECT_NEAR(gray_value, 0.67, 1e-9);
}

TEST(TextureTest, GetValueFromGrayScale) {
  Texture texture(0.5);
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
  Texture texture(ColorRGB(0.2, 0.4, 0.6));
  ColorRGB rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value.r, 0.2, 1e-9);
  EXPECT_NEAR(rgb_value.g, 0.4, 1e-9);
  EXPECT_NEAR(rgb_value.b, 0.6, 1e-9);

  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, toGrayscale(rgb_value), 1e-9);

  ColorRGBA rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value.r, 0.2, 1e-9);
  EXPECT_NEAR(rgba_value.g, 0.4, 1e-9);
  EXPECT_NEAR(rgba_value.b, 0.6, 1e-9);
  EXPECT_NEAR(rgba_value.a, 1.0, 1e-9);
}

TEST(TextureTest, GetValueFromRGBA) {
  Texture texture(ColorRGBA(0.1, 0.3, 0.5, 0.7));
  ColorRGBA rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value.r, 0.1, 1e-9);
  EXPECT_NEAR(rgba_value.g, 0.3, 1e-9);
  EXPECT_NEAR(rgba_value.b, 0.5, 1e-9);
  EXPECT_NEAR(rgba_value.a, 0.7, 1e-9);

  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, toGrayscale(rgba_value), 1e-9);

  ColorRGB rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value.r, 0.1, 1e-9);
  EXPECT_NEAR(rgb_value.g, 0.3, 1e-9);
  EXPECT_NEAR(rgb_value.b, 0.5, 1e-9);
}

TEST(TextureTest, ColorSpaceDefaultSRGB) {
  double gray = 0.5;
  Texture texture(gray);
  texture.setColorSpace(ColorSpace::sRGB);
  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_EQ(value, 0.5);
}

TEST(TextureTest, ColorSpaceConversionSRGBToLinear) {
  double gray = 0.5;
  Texture texture(gray);
  texture.setColorSpace(ColorSpace::RGB);
  double value = texture.getValue1d({0.0, 0.0});
  convertToLinearSpace(gray);
  EXPECT_NEAR(value, gray, 1e-9);
}

TEST(TextureTest, ColorSpaceConversionLinearToSRGB) {
  Texture texture(0.0);
  texture.setColorSpace(ColorSpace::RGB);
  double gray = 0.5;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::sRGB);

  double value = texture.getValue1d({0.0, 0.0});
  convertToSRGBSpace(gray);
  EXPECT_NEAR(value, gray, 1e-9);
}

TEST(TextureTest, FlipVertically) {
  ImageProperties properties = {2, 2, 1};
  double imageData[4] = {0.0, 0.1, 0.2, 0.3};
  Texture texture(imageData, properties);
  texture.flipVertically();

  double value1 = texture.getValue1d({0.0, 0.0});
  double value2 = texture.getValue1d({1.0, 0.0});
  double value3 = texture.getValue1d({0.0, 1.0});
  double value4 = texture.getValue1d({1.0, 1.0});

  EXPECT_NEAR(value1, 0.2, 1e-9);
  EXPECT_NEAR(value2, 0.3, 1e-9);
  EXPECT_NEAR(value3, 0.0, 1e-9);
  EXPECT_NEAR(value4, 0.1, 1e-9);
}

