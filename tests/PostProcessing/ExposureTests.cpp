#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Exposure.hpp"

TEST(ExposureToneMappingTest, ConvertToLDR_NegativeOrZeroInput_ReturnsZero) {
  ExposureToneMapping toneMapper(1.0);

  EXPECT_DOUBLE_EQ(toneMapper.convertToLDR(-1.0), 0.0);
  EXPECT_DOUBLE_EQ(toneMapper.convertToLDR(0.0), 0.0);
}

TEST(ExposureToneMappingTest, ConvertToLDR_PositiveInput_CorrectMapping) {
  ExposureToneMapping toneMapper(1.0);

  double input = 1.0;
  double expected = 1.0 - std::exp(-1.0);
  EXPECT_NEAR(toneMapper.convertToLDR(input), expected, 1e-9);

  input = 2.0;
  expected = 1.0 - std::exp(-2.0);
  EXPECT_NEAR(toneMapper.convertToLDR(input), expected, 1e-9);
}

TEST(ExposureToneMappingTest, ConvertToLDR_HighExposure_MoreCompression) {
  ExposureToneMapping toneMapperLow(0.5);
  ExposureToneMapping toneMapperHigh(2.0);

  double input = 1.0;

  double lowExposureOutput  = toneMapperLow.convertToLDR(input);
  double highExposureOutput = toneMapperHigh.convertToLDR(input);

  EXPECT_GT(highExposureOutput, lowExposureOutput);
}

TEST(ExposureToneMappingTest, ConvertToLDR_ColorRGB_ValuesMappedCorrectly) {
  ExposureToneMapping toneMapper(1.0);
  ColorRGB input{1.0, 2.0, 3.0};

  ColorRGB output = toneMapper.convertToLDR(input);

  EXPECT_NEAR(output.r, 1.0 - std::exp(-1.0), 1e-9);
  EXPECT_NEAR(output.g, 1.0 - std::exp(-2.0), 1e-9);
  EXPECT_NEAR(output.b, 1.0 - std::exp(-3.0), 1e-9);
}
