#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Reinhard.hpp"

TEST(ReinhardToneMappingTest, ValueZero) {
  ReinhardToneMapping* toneMapping = new ReinhardToneMapping();
  double value = 0.0;
  toneMapping->apply(value);
  EXPECT_DOUBLE_EQ(value, 0.0);
  delete toneMapping;
}

TEST(ReinhardToneMappingTest, ValuePositive) {
  ReinhardToneMapping toneMapping;
  double value = 1.0;
  toneMapping.apply(value);
  EXPECT_DOUBLE_EQ(value, 0.5);
}

TEST(ReinhardToneMappingTest, ValueLargePositive) {
  ReinhardToneMapping toneMapping;
  double value = 9.0;
  toneMapping.apply(value);
  EXPECT_DOUBLE_EQ(value, 9.0 / (1.0 + 9.0));
}

TEST(ReinhardToneMappingTest, ValueSmallPositive) {
  ReinhardToneMapping toneMapping;
  double value = 0.1;
  toneMapping.apply(value);
  EXPECT_DOUBLE_EQ(value, 0.1 / (1.0 + 0.1));
}

TEST(ReinhardToneMappingTest, ValueNegative) {
  ReinhardToneMapping toneMapping = ReinhardToneMapping();
  double value = -0.1;
  toneMapping.apply(value);
  EXPECT_DOUBLE_EQ(value, 0.0);

  value = -1.0;
  toneMapping.apply(value);
  EXPECT_DOUBLE_EQ(value, 0.0);
}
