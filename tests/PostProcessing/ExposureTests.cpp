#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Exposure.hpp"

#include "ImplementationParameters/ParameterTypes.hpp"

TEST(ExposureToneMappingTest, ApplyDefaultExposure) {
    Parameters params;
    ExposureToneMapping toneMapping(&params);
    toneMapping.updateParameters(&params);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_NEAR(value, 1.0 - std::exp(-1.0), 1e-10);
}

TEST(ExposureToneMappingTest, ApplyDefaultExposureWithNoParams) {
    Parameters params;
    ExposureToneMapping toneMapping(&params);

    Parameters new_params;
    toneMapping.updateParameters(&new_params);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_EQ(value, 1 - exp(-1.0));
}

TEST(ExposureToneMappingTest, ApplyCustomExposure) {
  Parameters params;
    ExposureToneMapping toneMapping(&params);
    params.setParameter("exposure", 2.0);
    toneMapping.updateParameters(&params);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_NEAR(value, 1.0 - std::exp(-2.0), 1e-10);
}

TEST(ExposureToneMappingTest, ApplyZeroExposure) {
  Parameters params;
    ExposureToneMapping toneMapping(&params);
    params.setParameter("exposure", 0.0);
    toneMapping.updateParameters(&params);

    double value = 1.0;
    toneMapping.apply(value);

    EXPECT_NEAR(value, 0.0, 1e-10);
}

TEST(ExposureToneMappingTest, ApplyNegativeExposure) {
  Parameters params;
    ExposureToneMapping toneMapping(&params);
    params.setParameter("exposure", -1.0);
    toneMapping.updateParameters(&params);

    double value = 1.0;
    toneMapping.apply(value);

    std::cout << params.get<DoubleParam>("exposure")->getValue() << '\n';
    std::cout << value << '\n';

    EXPECT_EQ(value, 0.0); // exp(positive) > 1, so result < 0
}
