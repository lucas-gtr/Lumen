#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/LuminanceReinhard.hpp"

// TEST(ReinhardToneMappingTest, ValueZero) {
//   LuminanceReinhardToneMapping toneMapping;
//   double value = 0.0;
//   toneMapping->apply(value);
//   EXPECT_DOUBLE_EQ(value, 0.0);
// }

// TEST(ReinhardToneMappingTest, ValuePositive) {
//   LuminanceReinhardToneMapping toneMapping;
//   double value = 1.0;
//   toneMapping.apply(value);
//   EXPECT_DOUBLE_EQ(value, 0.5);
// }

// TEST(ReinhardToneMappingTest, ValueLargePositive) {
//   LuminanceReinhardToneMapping toneMapping;
//   double value = 9.0;
//   toneMapping.apply(value);
//   EXPECT_DOUBLE_EQ(value, 9.0 / (1.0 + 9.0));
// }

// TEST(ReinhardToneMappingTest, ValueSmallPositive) {
//   LuminanceReinhardToneMapping toneMapping;
//   double value = 0.1;
//   toneMapping.apply(value);
//   EXPECT_DOUBLE_EQ(value, 0.1 / (1.0 + 0.1));
// }

// TEST(ReinhardToneMappingTest, ValueNegative) {
//   LuminanceReinhardToneMapping toneMapping;
//   double value = -0.1;
//   toneMapping.apply(value);
//   EXPECT_DOUBLE_EQ(value, 0.0);

//   value = -1.0;
//   toneMapping.apply(value);
//   EXPECT_DOUBLE_EQ(value, 0.0);
// }
