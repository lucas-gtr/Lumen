#include <gtest/gtest.h>
#include <Eigen/Dense>

#include "Core/ColorUtils.hpp"
#include "Surface/Texture.hpp"

TEST(TextureTest, ConstructFromDouble) {
  Texture texture(0.5);
  EXPECT_NEAR(texture.getValue1d({0.0, 0.0}), 0.5, 1e-9);
}

TEST(TextureTest, ConstructFromVector3d) {
  Texture texture(Eigen::Vector3d(0.2, 0.4, 0.6));
  Eigen::Vector3d value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value[0], 0.2, 1e-9);
  EXPECT_NEAR(value[1], 0.4, 1e-9);
  EXPECT_NEAR(value[2], 0.6, 1e-9);
}

TEST(TextureTest, ConstructFromVector4d) {
  Texture texture(Eigen::Vector4d(0.1, 0.3, 0.5, 0.7));
  Eigen::Vector4d value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(value[0], 0.1, 1e-9);
  EXPECT_NEAR(value[1], 0.3, 1e-9);
  EXPECT_NEAR(value[2], 0.5, 1e-9);
  EXPECT_NEAR(value[3], 0.7, 1e-9);
}

TEST(TextureTest, ConstructorFromImageData) {
  unsigned char imageData[4] = {255, 0, 255, 128};
  ImageProperties properties = {1, 1, 4};
  Texture texture(imageData, properties);
  Eigen::Vector4d value = texture.getValue4d({0.0, 0.0});
  EXPECT_EQ(value[0], 1.0);
  EXPECT_EQ(value[1], 0.0);
  EXPECT_EQ(value[2], 1.0);
  EXPECT_NEAR(value[3], 0.5, 1e-2);
}

TEST(TextureTest, ConstructorFromDoubleArray) {
  double imageData[4] = {1.0, 0.0, 0.0, 1.0};
  ImageProperties properties = {1, 1, 4};
  Texture texture(imageData, properties);
  Eigen::Vector4d value = texture.getValue4d({0.0, 0.0});
  EXPECT_EQ(value[0], 1.0);
  EXPECT_EQ(value[1], 0.0);
  EXPECT_EQ(value[2], 0.0);
  EXPECT_EQ(value[3], 1.0);
}

TEST(TextureTest, CopyConstructor) {
  Texture original(Eigen::Vector3d(0.6, 0.7, 0.8));
  Texture copy = original;
  Eigen::Vector3d value = copy.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value[0], 0.6, 1e-9);
  EXPECT_NEAR(value[1], 0.7, 1e-9);
  EXPECT_NEAR(value[2], 0.8, 1e-9);
}

TEST(TextureTest, MoveConstructor) {
  Texture original(Eigen::Vector3d(0.6, 0.7, 0.8));
  Texture moved = std::move(original);
  Eigen::Vector3d value = moved.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value[0], 0.6, 1e-9);
  EXPECT_NEAR(value[1], 0.7, 1e-9);
  EXPECT_NEAR(value[2], 0.8, 1e-9);
}

TEST(TextureTest, CopyAssignment) {
  Texture original(Eigen::Vector3d(0.6, 0.7, 0.8));
  Texture copy(0.0);
  copy = original;
  
  original.setValue(Eigen::Vector3d(0.1, 0.2, 0.3));
  Eigen::Vector3d original_value = original.getValue3d({0.0, 0.0});
  EXPECT_NEAR(original_value[0], 0.1, 1e-9);
  EXPECT_NEAR(original_value[1], 0.2, 1e-9);
  EXPECT_NEAR(original_value[2], 0.3, 1e-9);
  Eigen::Vector3d copy_value = copy.getValue3d({0.0, 0.0});
  EXPECT_NEAR(copy_value[0], 0.6, 1e-9);
  EXPECT_NEAR(copy_value[1], 0.7, 1e-9);
  EXPECT_NEAR(copy_value[2], 0.8, 1e-9);
}

TEST(TextureTest, MoveAssignment) {
  Texture original(Eigen::Vector3d(0.6, 0.7, 0.8));
  Texture moved(0.0);
  moved = std::move(original);

  Eigen::Vector3d moved_value = moved.getValue3d({0.0, 0.0});
  EXPECT_NEAR(moved_value[0], 0.6, 1e-9);
  EXPECT_NEAR(moved_value[1], 0.7, 1e-9);
  EXPECT_NEAR(moved_value[2], 0.8, 1e-9);

  EXPECT_NE(&original, &moved);
}

TEST(TextureTest, SetGrayValue) {
  Texture texture(0.0);
  texture.setValue(0.5);
  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(value, 0.5, 1e-9);
}

TEST(TextureTest, SetRGBValue) {
  Texture texture(Eigen::Vector3d(0.0, 0.0, 0.0));
  texture.setValue(Eigen::Vector3d(0.1, 0.2, 0.3));
  Eigen::Vector3d value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(value[0], 0.1, 1e-9);
  EXPECT_NEAR(value[1], 0.2, 1e-9);
  EXPECT_NEAR(value[2], 0.3, 1e-9);
}

TEST(TextureTest, SetRGBAValue) {
  Texture texture(Eigen::Vector4d(0.0, 0.0, 0.0, 0.0));
  texture.setValue(Eigen::Vector4d(0.1, 0.2, 0.3, 0.4));
  Eigen::Vector4d value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(value[0], 0.1, 1e-9);
  EXPECT_NEAR(value[1], 0.2, 1e-9);
  EXPECT_NEAR(value[2], 0.3, 1e-9);
  EXPECT_NEAR(value[3], 0.4, 1e-9);
}

TEST(TextureTest, SetImageData) {
  Texture texture(1.0);
  
  double imageData[4] = {1.0, 0.0, 0.0, 1.0};
  ImageProperties properties = {1, 1, 4};
  texture.setImageData(imageData, properties);

  Eigen::Vector4d value = texture.getValue4d({0.0, 0.0});
  EXPECT_EQ(value[0], 1.0);
  EXPECT_EQ(value[1], 0.0);
  EXPECT_EQ(value[2], 0.0);
  EXPECT_EQ(value[3], 1.0);
}

TEST(TextureTest, SetBorderColor) {
  Eigen::Vector4d borderColor_4d(1.0, 0.5, 0.5, 1.0);
  Texture texture_4d(Eigen::Vector4d(0.0, 0.0, 0.0, 0.0));
  texture_4d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_4d.setBorderColor(borderColor_4d);
  Eigen::Vector4d value = texture_4d.getValue4d({-1.0, -1.0});
  EXPECT_EQ(value[0], 1.0);
  EXPECT_EQ(value[1], 0.5);
  EXPECT_EQ(value[2], 0.5);
  EXPECT_EQ(value[3], 1.0);

  Eigen::Vector3d borderColor_3d(1.0, 0.5, 0.5);
  Texture texture_3d(Eigen::Vector3d(0.0, 0.0, 0.0));
  texture_3d.setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  texture_3d.setBorderColor(borderColor_3d);
  Eigen::Vector3d rgb_value = texture_3d.getValue3d({-1.0, -1.0});
  EXPECT_EQ(rgb_value[0], 1.0);
  EXPECT_EQ(rgb_value[1], 0.5);
  EXPECT_EQ(rgb_value[2], 0.5);

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

  Eigen::Vector3d rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value[0], 0.5, 1e-9);
  EXPECT_NEAR(rgb_value[1], 0.5, 1e-9);
  EXPECT_NEAR(rgb_value[2], 0.5, 1e-9);

  Eigen::Vector4d rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value[0], 0.5, 1e-9);
  EXPECT_NEAR(rgba_value[1], 0.5, 1e-9);
  EXPECT_NEAR(rgba_value[2], 0.5, 1e-9);
  EXPECT_NEAR(rgba_value[3], 1.0, 1e-9);
}

TEST(TextureTest, GetValueFromRGB) {
  Texture texture(Eigen::Vector3d(0.2, 0.4, 0.6));
  Eigen::Vector3d rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value[0], 0.2, 1e-9);
  EXPECT_NEAR(rgb_value[1], 0.4, 1e-9);
  EXPECT_NEAR(rgb_value[2], 0.6, 1e-9);

  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, toGrayscale(rgb_value), 1e-9);

  Eigen::Vector4d rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value[0], 0.2, 1e-9);
  EXPECT_NEAR(rgba_value[1], 0.4, 1e-9);
  EXPECT_NEAR(rgba_value[2], 0.6, 1e-9);
  EXPECT_NEAR(rgba_value[3], 1.0, 1e-9);
}

TEST(TextureTest, GetValueFromRGBA) {
  Texture texture(Eigen::Vector4d(0.1, 0.3, 0.5, 0.7));
  Eigen::Vector4d rgba_value = texture.getValue4d({0.0, 0.0});
  EXPECT_NEAR(rgba_value[0], 0.1, 1e-9);
  EXPECT_NEAR(rgba_value[1], 0.3, 1e-9);
  EXPECT_NEAR(rgba_value[2], 0.5, 1e-9);
  EXPECT_NEAR(rgba_value[3], 0.7, 1e-9);

  double gray_value = texture.getValue1d({0.0, 0.0});
  EXPECT_NEAR(gray_value, toGrayscale(rgba_value), 1e-9);

  Eigen::Vector3d rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value[0], 0.1, 1e-9);
  EXPECT_NEAR(rgb_value[1], 0.3, 1e-9);
  EXPECT_NEAR(rgb_value[2], 0.5, 1e-9);
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

