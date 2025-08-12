#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/WhitePointReinhard.hpp"

TEST(WhitePointReinhardToneMappingTest, ConvertToLDR_ColorRGB_Zero) {
  WhitePointReinhardToneMapping tone_mapper(1.0);
  ColorRGB input = {0.0, 0.0, 0.0};
  ColorRGB result = tone_mapper.convertToLDR(input);
  EXPECT_DOUBLE_EQ(result.r, 0.0);
  EXPECT_DOUBLE_EQ(result.g, 0.0);
  EXPECT_DOUBLE_EQ(result.b, 0.0);
}

TEST(WhitePointReinhardToneMappingTest, ConvertToLDR_ColorRGB_WhitePoint1) {
  WhitePointReinhardToneMapping tone_mapper(1.0);
  ColorRGB input = {1.0, 2.0, 3.0};
  double luminance = input.luminance();
  double factor = (1.0 + luminance / (1.0 * 1.0)) / (1.0 + luminance);
  ColorRGB expected = input * factor;
  ColorRGB result = tone_mapper.convertToLDR(input);
  EXPECT_DOUBLE_EQ(result.r, expected.r);
  EXPECT_DOUBLE_EQ(result.g, expected.g);
  EXPECT_DOUBLE_EQ(result.b, expected.b);
}

TEST(WhitePointReinhardToneMappingTest, ConvertToLDR_ColorRGB_CustomWhitePoint) {
  WhitePointReinhardToneMapping tone_mapper(2.0);
  ColorRGB input = {0.5, 0.2, 0.1};
  double luminance = input.luminance();
  double factor = (1.0 + luminance / (2.0 * 2.0)) / (1.0 + luminance);
  ColorRGB expected = input * factor;
  ColorRGB result = tone_mapper.convertToLDR(input);
  EXPECT_NEAR(result.r, expected.r, 1e-9);
  EXPECT_NEAR(result.g, expected.g, 1e-9);
  EXPECT_NEAR(result.b, expected.b, 1e-9);
}
