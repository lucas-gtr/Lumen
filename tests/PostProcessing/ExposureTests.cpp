#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Exposure.hpp"


TEST(ExposureToneMappingTest, ApplyDefaultExposure) {
    ExposureToneMapping toneMapping(1.0);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_NEAR(value, 1.0 - std::exp(-1.0), 1e-10);
}

TEST(ExposureToneMappingTest, ApplyCustomExposure) {
    ExposureToneMapping toneMapping(2.0);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_NEAR(value, 1.0 - std::exp(-2.0), 1e-10);
}

TEST(ExposureToneMappingTest, ApplyZeroExposure) {
    ExposureToneMapping toneMapping(0.0);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_NEAR(value, 0.0, 1e-10);
}