#include <gtest/gtest.h>

#include "Core/ColorUtils.hpp"
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

  EXPECT_EQ(texture_4d.getBorderColor(), borderColor_4d);

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
  EXPECT_NEAR(gray_value, toGrayscale(rgb_value), 1e-9);

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
  EXPECT_NEAR(gray_value, toGrayscale(rgba_value), 1e-9);

  ColorRGB rgb_value = texture.getValue3d({0.0, 0.0});
  EXPECT_NEAR(rgb_value.r, 0.1, 1e-9);
  EXPECT_NEAR(rgb_value.g, 0.3, 1e-9);
  EXPECT_NEAR(rgb_value.b, 0.5, 1e-9);
}

TEST(TextureTest, ColorSpaceDefaultSRGB) {
  double gray = 0.5;
  Texture texture;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::sRGB);
  double value = texture.getValue1d({0.0, 0.0});
  EXPECT_EQ(value, 0.5);
}

TEST(TextureTest, ColorSpaceConversionSRGBToLinear) {
  double gray = 0.5;
  Texture texture;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::Linear);
  double value = texture.getValue1d({0.0, 0.0});
  convertToLinearSpace(gray);
  EXPECT_NEAR(value, gray, 1e-9);
}

TEST(TextureTest, ColorSpaceConversionLinearToSRGB) {
  Texture texture;
  texture.setValue(0.0);
  texture.setColorSpace(ColorSpace::Linear);
  double gray = 0.5;
  texture.setValue(gray);
  texture.setColorSpace(ColorSpace::sRGB);

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

