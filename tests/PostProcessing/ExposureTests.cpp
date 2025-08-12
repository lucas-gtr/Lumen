#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Exposure.hpp"

TEST(ExposureToneMappingTest, ConvertToLDR_ColorRGB_ValuesMappedCorrectly) {
  ExposureToneMapping toneMapper(1.0);
  ColorRGB input{1.0, 2.0, 3.0};

  ColorRGB output = toneMapper.convertToLDR(input);

  EXPECT_NEAR(output.r, 1.0 - std::exp(-1.0), 1e-9);
  EXPECT_NEAR(output.g, 1.0 - std::exp(-2.0), 1e-9);
  EXPECT_NEAR(output.b, 1.0 - std::exp(-3.0), 1e-9);
}
