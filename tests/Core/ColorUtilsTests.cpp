#include "Core/ColorUtils.hpp"
#include <gtest/gtest.h>

TEST(ColorUtilsTest, Vector3dToGrayscale) {
    Eigen::Vector3d color(0.5, 0.2, 0.3);
    double expectedGrayscale = RED_CHANNEL * 0.5 + GREEN_CHANNEL * 0.2 + BLUE_CHANNEL * 0.3;
    EXPECT_DOUBLE_EQ(toGrayscale(color), expectedGrayscale);
}

TEST(ColorUtilsTest, Vector4dToGrayscale) {
    Eigen::Vector4d color(0.5, 0.2, 0.3, 1.0);
    double expectedGrayscale = RED_CHANNEL * 0.5 + GREEN_CHANNEL * 0.2 + BLUE_CHANNEL * 0.3;
    EXPECT_DOUBLE_EQ(toGrayscale(color), expectedGrayscale);
}

TEST(ColorUtilsTest, GrayscaleToRGB) {
    double grayscale = 0.4;
    Eigen::Vector3d expectedColor(0.4, 0.4, 0.4);
    EXPECT_TRUE(toRGB(grayscale).isApprox(expectedColor));
}

TEST(ColorUtilsTest, Vector4dToRGB) {
    Eigen::Vector4d color(0.5, 0.6, 0.7, 1.0);
    Eigen::Vector3d expectedColor(0.5, 0.6, 0.7);
    EXPECT_TRUE(toRGB(color).isApprox(expectedColor));
}

TEST(ColorUtilsTest, GrayscaleToRGBA) {
    double grayscale = 0.8;
    Eigen::Vector4d expectedColor(0.8, 0.8, 0.8, 1.0);
    EXPECT_TRUE(toRGBA(grayscale).isApprox(expectedColor));
}

TEST(ColorUtilsTest, Vector3dToRGBAFrom) {
    Eigen::Vector3d color(0.1, 0.2, 0.3);
    Eigen::Vector4d expectedColor(0.1, 0.2, 0.3, 1.0);
    EXPECT_TRUE(toRGBA(color).isApprox(expectedColor));
}

TEST(ColorSpaceConversionTest, ConvertToSRGBSpaceBelowThreshold) {
    double value = 0.002;
    double expected = value * RGB_THRESHOLD_FACTOR;
    convertToSRGBSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorSpaceConversionTest, ConvertToSRGBSpaceAboveThreshold) {
    double value = 0.5;
    double expected = (std::pow(0.5, RGB_GAMMA) * RGB_FACTOR) - COLOR_SPACE_ADDENDUM;
    convertToSRGBSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorSpaceConversionTest, ConvertToLinearSpaceBelowThreshold) {
    double value = 0.02;
    double expected = value * SRGB_THRESHOLD_FACTOR;
    convertToLinearSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorSpaceConversionTest, ConvertToLinearSpaceAboveThreshold) {
    double value = 0.5;
    double expected = std::pow((0.5 + COLOR_SPACE_ADDENDUM) * SRGB_FACTOR, SRGB_GAMMA);
    convertToLinearSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorSpaceConversionTest, ConvertToSRGBAndBack) {
    double original = 0.5;
    double converted = original;
    convertToSRGBSpace(converted);
    convertToLinearSpace(converted);
    EXPECT_NEAR(converted, original, 1e-6);
}

TEST(ColorSpaceConversionTest, ConvertToLinearAndBack) {
    double original = 0.5;
    double converted = original;
    convertToLinearSpace(converted);
    convertToSRGBSpace(converted);
    EXPECT_NEAR(converted, original, 1e-6);
}
