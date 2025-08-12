#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Uncharted2.hpp"

TEST(Uncharted2ToneMappingTest, PartialScalar_KnownValue) {
    double result = Uncharted2ToneMapping::GetUncharted2ToneMapPartial(1.0);

    double expected = 0.22065727699530524;
    EXPECT_NEAR(result, expected, 1e-6);
}

TEST(Uncharted2ToneMappingTest, PartialScalar_Zero) {
    double result = Uncharted2ToneMapping::GetUncharted2ToneMapPartial(0.0);
    double expected = 0.0;
    EXPECT_NEAR(result, expected, 1e-6);
}

TEST(Uncharted2ToneMappingTest, PartialVector_KnownValue) {
    ColorRGB input{1.0, 0.5, 0.25};
    ColorRGB result = Uncharted2ToneMapping::GetUncharted2ToneMapPartial(input);
    ColorRGB expected{0.22065728, 0.12470024, 0.0664523};
    EXPECT_NEAR(result.r, expected.r, 1e-6);
    EXPECT_NEAR(result.g, expected.g, 1e-6);
    EXPECT_NEAR(result.b, expected.b, 1e-6);
}

TEST(Uncharted2ToneMappingTest, ConvertToLDR_Exposure2_WhitePoint11_2) {
    Uncharted2ToneMapping tm(2.0, 11.2);
    ColorRGB input{1.0, 0.5, 0.25};
    ColorRGB result = tm.convertToLDR(input);

    ColorRGB expected{0.49291855, 0.30430056, 0.17196964};
    EXPECT_NEAR(result.r, expected.r, 1e-6);
    EXPECT_NEAR(result.g, expected.g, 1e-6);
    EXPECT_NEAR(result.b, expected.b, 1e-6);
}

TEST(Uncharted2ToneMappingTest, ConvertToLDR_Gray18) {
    Uncharted2ToneMapping tm(1.0, 11.2);
    ColorRGB input{0.18, 0.18, 0.18};
    ColorRGB result = tm.convertToLDR(input);

    ColorRGB expected{0.06710983, 0.06710983, 0.06710983};
    EXPECT_NEAR(result.r, expected.r, 1e-6);
    EXPECT_NEAR(result.g, expected.g, 1e-6);
    EXPECT_NEAR(result.b, expected.b, 1e-6);
}

TEST(Uncharted2ToneMappingTest, ConvertToLDR_Black) {
    Uncharted2ToneMapping tm(1.0, 11.2);
    ColorRGB input{0.0, 0.0, 0.0};
    ColorRGB result = tm.convertToLDR(input);
    EXPECT_NEAR(result.r, 0.0, 1e-12);
    EXPECT_NEAR(result.g, 0.0, 1e-12);
    EXPECT_NEAR(result.b, 0.0, 1e-12);
}

TEST(Uncharted2ToneMappingTest, ConvertToLDR_HighSaturation) {
    Uncharted2ToneMapping tm(1.0, 11.2);
    ColorRGB input{10.0, 1.0, 0.0};
    ColorRGB result = tm.convertToLDR(input);

    ColorRGB expected{0.97391543, 0.30430056, 0.0}; // Tonemapped avec compression
    EXPECT_NEAR(result.r, expected.r, 1e-6);
    EXPECT_NEAR(result.g, expected.g, 1e-6);
    EXPECT_NEAR(result.b, expected.b, 1e-6);
}
