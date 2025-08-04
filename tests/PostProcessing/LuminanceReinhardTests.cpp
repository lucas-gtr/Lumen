#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/LuminanceReinhard.hpp"

TEST(LuminanceReinhardToneMappingTests, ConvertScalarToLDR_Zero) {
  LuminanceReinhardToneMapping toneMapping;
  EXPECT_DOUBLE_EQ(toneMapping.convertToLDR(0.0), 0.0);
}

TEST(LuminanceReinhardToneMappingTests, ConvertScalarToLDR_Negative) {
  LuminanceReinhardToneMapping toneMapping;
  EXPECT_DOUBLE_EQ(toneMapping.convertToLDR(-5.0), 0.0);
}

TEST(LuminanceReinhardToneMappingTests, ConvertScalarToLDR_Positive) {
  LuminanceReinhardToneMapping toneMapping;
  EXPECT_NEAR(toneMapping.convertToLDR(1.0), 0.5, 1e-9);
  EXPECT_NEAR(toneMapping.convertToLDR(4.0), 0.8, 1e-9);
  EXPECT_NEAR(toneMapping.convertToLDR(9.0), 0.9, 1e-9);
}

TEST(LuminanceReinhardToneMappingTests, ConvertColorToLDR_White) {
  LuminanceReinhardToneMapping toneMapping;
  ColorRGB white(1.0, 1.0, 1.0);
  ColorRGB result = toneMapping.convertToLDR(white);

  double luminance = white.luminance();
  ColorRGB expected = lerp(white / (1.0 + luminance), white / (ColorRGB(1.0) + white), white / (ColorRGB(1.0) + white));

  EXPECT_NEAR(result.r, expected.r, 1e-9);
  EXPECT_NEAR(result.g, expected.g, 1e-9);
  EXPECT_NEAR(result.b, expected.b, 1e-9);
}

TEST(LuminanceReinhardToneMappingTests, ConvertColorToLDR_Black) {
  LuminanceReinhardToneMapping toneMapping;
  ColorRGB black(0.0, 0.0, 0.0);
  ColorRGB result = toneMapping.convertToLDR(black);

  EXPECT_DOUBLE_EQ(result.r, 0.0);
  EXPECT_DOUBLE_EQ(result.g, 0.0);
  EXPECT_DOUBLE_EQ(result.b, 0.0);
}

TEST(LuminanceReinhardToneMappingTests, ConvertColorToLDR_Custom) {
  LuminanceReinhardToneMapping toneMapping;
  ColorRGB color(0.5, 1.0, 2.0);
  ColorRGB result = toneMapping.convertToLDR(color);

  double luminance = color.luminance();
  ColorRGB ldr_color = color / (ColorRGB(1.0) + color);
  ColorRGB expected = lerp(color / (1.0 + luminance), ldr_color, ldr_color);

  EXPECT_NEAR(result.r, expected.r, 1e-9);
  EXPECT_NEAR(result.g, expected.g, 1e-9);
  EXPECT_NEAR(result.b, expected.b, 1e-9);
}
