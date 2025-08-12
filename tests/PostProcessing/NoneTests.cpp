#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/None.hpp"

TEST(NoToneMappingTests, ConvertColorAllComponentsClampedIndividually) {
  NoToneMapping tone_mapping;
  ColorRGB input{-0.2, 0.5, 1.8};
  ColorRGB output = tone_mapping.convertToLDR(input);

  EXPECT_DOUBLE_EQ(output.r, 0.0);
  EXPECT_DOUBLE_EQ(output.g, 0.5);
  EXPECT_DOUBLE_EQ(output.b, 1.0);
}

TEST(NoToneMappingTests, ConvertColorAllComponentsInRangeUnchanged) {
  NoToneMapping tone_mapping;
  ColorRGB input{0.1, 0.5, 0.9};
  ColorRGB output = tone_mapping.convertToLDR(input);

  EXPECT_DOUBLE_EQ(output.r, 0.1);
  EXPECT_DOUBLE_EQ(output.g, 0.5);
  EXPECT_DOUBLE_EQ(output.b, 0.9);
}
