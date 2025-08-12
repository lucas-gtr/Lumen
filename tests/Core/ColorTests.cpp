#include "Core/Color.hpp"
#include <gtest/gtest.h>

TEST(ColorTest, DefaultConstructorsRGB) {
  ColorRGB c1;
  EXPECT_DOUBLE_EQ(c1.r, 0.0);
  EXPECT_DOUBLE_EQ(c1.g, 0.0);
  EXPECT_DOUBLE_EQ(c1.b, 0.0);

  ColorRGB c2(1.0, 0.5, 0.25);
  EXPECT_DOUBLE_EQ(c2.r, 1.0);
  EXPECT_DOUBLE_EQ(c2.g, 0.5);
  EXPECT_DOUBLE_EQ(c2.b, 0.25);

  ColorRGB c3(0.8);
  EXPECT_DOUBLE_EQ(c3.r, 0.8);
  EXPECT_DOUBLE_EQ(c3.g, 0.8);
  EXPECT_DOUBLE_EQ(c3.b, 0.8);
}

TEST(ColorTest, OperatorsRGB) {
  ColorRGB c1(1.0, 2.0, 3.0);
  ColorRGB c2(0.5, 1.0, 1.5);

  EXPECT_EQ(c1 + c2, ColorRGB(1.5, 3.0, 4.5));
  EXPECT_EQ(c1 - c2, ColorRGB(0.5, 1.0, 1.5));
  EXPECT_EQ(c1 * 2.0, ColorRGB(2.0, 4.0, 6.0));
  EXPECT_EQ(2.0 * c1, ColorRGB(2.0, 4.0, 6.0));
  EXPECT_EQ(c1 * c2, ColorRGB(0.5, 2.0, 4.5));
  EXPECT_EQ(c1 / 2.0, ColorRGB(0.5, 1.0, 1.5));
  EXPECT_EQ(c1 / c2, ColorRGB(2.0, 2.0, 2.0));
}

TEST(ColorTest, CompoundAssignmentOperatorsRGB) {
  ColorRGB c(1.0, 2.0, 3.0);
  c += ColorRGB(1.0, 0.0, -1.0);
  EXPECT_EQ(c, ColorRGB(2.0, 2.0, 2.0));

  c -= ColorRGB(1.0, 1.0, 1.0);
  EXPECT_EQ(c, ColorRGB(1.0, 1.0, 1.0));

  c *= 2.0;
  EXPECT_EQ(c, ColorRGB(2.0, 2.0, 2.0));

  c *= ColorRGB(2.0, 0.5, 1.0);
  EXPECT_EQ(c, ColorRGB(4.0, 1.0, 2.0));

  c /= 2.0;
  EXPECT_EQ(c, ColorRGB(2.0, 0.5, 1.0));

  c /= ColorRGB(2.0, 0.5, 1.0);
  EXPECT_EQ(c, ColorRGB(1.0, 1.0, 1.0));
}

TEST(ColorTest, ClampAndMinMaxComponentRGB) {
  ColorRGB c(2.0, -1.0, 1.0);
  c.clamp(0.0, 1.0);
  EXPECT_EQ(c, ColorRGB(1.0, 0.0, 1.0));

  EXPECT_DOUBLE_EQ(ColorRGB(0.1, 0.9, 0.5).maxComponent(), 0.9);
  EXPECT_DOUBLE_EQ(ColorRGB(0.1, 0.9, 0.5).minComponent(), 0.1);
}

TEST(ColorTest, DivisionByZeroRGB) {
  ColorRGB c(1.0, 1.0, 1.0);
  EXPECT_THROW(c / 0.0, std::runtime_error);
  EXPECT_THROW(c /= 0.0, std::runtime_error);
  EXPECT_THROW(c / ColorRGB(1.0, 0.0, 1.0), std::runtime_error);
  EXPECT_THROW(c /= ColorRGB(1.0, 0.0, 1.0), std::runtime_error);
}

TEST(ColorTest, RGBLinearSpaceBackSRGB) {
  ColorRGB color(0.3, 0.5, 0.7);
  ColorRGB linear_color = color.toLinearSpace();
  ColorRGB srgb_color   = linear_color.toSRGBSpace();
  EXPECT_NEAR(srgb_color.r, color.r, 1e-6);
  EXPECT_NEAR(srgb_color.g, color.g, 1e-6);
  EXPECT_NEAR(srgb_color.b, color.b, 1e-6);
}

TEST(ColorTest, ConstructorsRGBA) {
  ColorRGBA c1;
  EXPECT_DOUBLE_EQ(c1.r, 0.0);
  EXPECT_DOUBLE_EQ(c1.a, 1.0);

  ColorRGBA c2(1.0, 0.5, 0.25, 0.75);
  EXPECT_DOUBLE_EQ(c2.r, 1.0);
  EXPECT_DOUBLE_EQ(c2.g, 0.5);
  EXPECT_DOUBLE_EQ(c2.b, 0.25);
  EXPECT_DOUBLE_EQ(c2.a, 0.75);

  ColorRGBA c3(ColorRGB(0.1, 0.2, 0.3), 0.5);
  EXPECT_EQ(c3, ColorRGBA(0.1, 0.2, 0.3, 0.5));
}

TEST(ColorTest, OperatorsRGBA) {
  ColorRGBA a(1, 2, 3, 4);
  ColorRGBA b(2, 3, 4, 5);
  ColorRGB c(1, 1, 1);

  EXPECT_EQ(a + b, ColorRGBA(3, 5, 7, 9));
  EXPECT_EQ(a - b, ColorRGBA(-1, -1, -1, -1));
  EXPECT_EQ(a * 2.0, ColorRGBA(2, 4, 6, 8));
  EXPECT_EQ(2.0 * a, ColorRGBA(2, 4, 6, 8));
  EXPECT_EQ(a * c, ColorRGBA(1, 2, 3, 4));
  EXPECT_EQ(a / 2.0, ColorRGBA(0.5, 1, 1.5, 2));
  EXPECT_EQ(a / b, ColorRGBA(0.5, 2.0 / 3.0, 0.75, 0.8));
}

TEST(ColorTest, CompoundAssignmentOperatorsRGBA) {
  ColorRGBA c(1.0, 2.0, 3.0, 1.0);
  c += ColorRGBA(1.0, 0.0, -1.0, 1.0);
  EXPECT_EQ(c, ColorRGBA(2.0, 2.0, 2.0, 2.0));

  c -= ColorRGBA(1.0, 1.0, 1.0, 1.0);
  EXPECT_EQ(c, ColorRGBA(1.0, 1.0, 1.0, 1.0));

  c *= 2.0;
  EXPECT_EQ(c, ColorRGBA(2.0, 2.0, 2.0, 2.0));

  c *= ColorRGBA(0.5, 0.5, 0.5, 0.5);
  EXPECT_EQ(c, ColorRGBA(1.0, 1.0, 1.0, 1.0));

  c /= 2.0;
  EXPECT_EQ(c, ColorRGBA(0.5, 0.5, 0.5, 0.5));

  c /= ColorRGBA(0.5, 0.5, 0.5, 0.5);
  EXPECT_EQ(c, ColorRGBA(1.0, 1.0, 1.0, 1.0));
}

TEST(ColorTest, ClampAndMinMaxComponentRGBA) {
  ColorRGBA c(2.0, -1.0, 1.0, 10.0);
  c.clamp(0.0, 1.0);
  EXPECT_EQ(c, ColorRGBA(1.0, 0.0, 1.0, 1.0));

  EXPECT_DOUBLE_EQ(ColorRGBA(0.1, 0.9, 0.5, 0.2).maxComponent(), 0.9);
  EXPECT_DOUBLE_EQ(ColorRGBA(0.1, 0.9, 0.5, 0.2).minComponent(), 0.1);
}

TEST(ColorTest, DivisionByZeroRGBA) {
  ColorRGBA c(1.0, 1.0, 1.0, 1.0);
  EXPECT_THROW(c / 0.0, std::runtime_error);
  EXPECT_THROW(c /= 0.0, std::runtime_error);
  EXPECT_THROW(c / ColorRGBA(0.0, 1.0, 1.0, 1.0), std::runtime_error);
  EXPECT_THROW(c /= ColorRGBA(0.0, 1.0, 1.0, 1.0), std::runtime_error);
}

TEST(ColorTest, RGBAtoRGBandBack) {
  ColorRGBA rgba(0.1, 0.2, 0.3, 0.4);
  ColorRGB  rgb = ColorRGB(rgba);
  EXPECT_EQ(rgb, ColorRGB(0.1, 0.2, 0.3));

  ColorRGBA converted = rgb + rgba;
  EXPECT_EQ(converted, ColorRGBA(0.2, 0.4, 0.6, 0.4));
}

TEST(ColorTest, ColorToGrayscale) {
    ColorRGB color(0.5, 0.2, 0.3);
    double expectedGrayscale = GRAY_RED_CHANNEL * 0.5 + GRAY_GREEN_CHANNEL * 0.2 + GRAY_BLUE_CHANNEL * 0.3;
    EXPECT_DOUBLE_EQ(color.grayscale(), expectedGrayscale);
}

TEST(ColorTest, Vector4dToGrayscale) {
    ColorRGBA color(0.5, 0.2, 0.3, 1.0);
    double expectedGrayscale = GRAY_RED_CHANNEL * 0.5 + GRAY_GREEN_CHANNEL * 0.2 + GRAY_BLUE_CHANNEL * 0.3;
    EXPECT_DOUBLE_EQ(color.grayscale(), expectedGrayscale);
}

TEST(ColorTest, GrayscaleToRGB) {
    double grayscale = 0.4;
    ColorRGB expectedColor(0.4, 0.4, 0.4);
    EXPECT_EQ(ColorRGB(grayscale), expectedColor);
}

TEST(ColorTest, Vector4dToRGB) {
    ColorRGBA color(0.5, 0.6, 0.7, 1.0);
    ColorRGB expectedColor(0.5, 0.6, 0.7);
    EXPECT_EQ(color.toRGB(), expectedColor);
    EXPECT_EQ(ColorRGB(color), expectedColor);
}

TEST(ColorTest, GrayscaleToRGBA) {
    double grayscale = 0.8;
    ColorRGBA expectedColor(0.8, 0.8, 0.8, 1.0);
    EXPECT_EQ(ColorRGBA(grayscale), expectedColor);
}

TEST(ColorTest, Vector3dToRGBAFrom) {
    ColorRGB color(0.1, 0.2, 0.3);
    ColorRGBA expectedColor(0.1, 0.2, 0.3, 1.0);
    EXPECT_EQ(color.toRGBA(), expectedColor);
    EXPECT_EQ(ColorRGBA(color), expectedColor);
}

TEST(ColorTest, ConvertToSRGBSpaceBelowThreshold) {
    double value = 0.002;
    double expected = value * RGB_THRESHOLD_FACTOR;
    convertToSRGBSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorTest, ConvertToSRGBSpaceAboveThreshold) {
    double value = 0.5;
    double expected = (std::pow(0.5, RGB_GAMMA) * RGB_FACTOR) - COLOR_SPACE_ADDENDUM;
    convertToSRGBSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorTest, ConvertToLinearSpaceBelowThreshold) {
    double value = 0.02;
    double expected = value * SRGB_THRESHOLD_FACTOR;
    convertToLinearSpace(value);
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorTest, ConvertToLinearSpaceAboveThreshold) {
    double value = 0.5;
    double expected = std::pow((0.5 + COLOR_SPACE_ADDENDUM) * SRGB_FACTOR, SRGB_GAMMA);
    convertToLinearSpace(value);
    std::cout << "Expected: " << expected << ", Value: " << value << std::endl;
    EXPECT_NEAR(value, expected, 1e-7);
}

TEST(ColorTest, ConvertToSRGBAndBack) {
    double original = 0.5;
    double converted = original;
    convertToSRGBSpace(converted);
    convertToLinearSpace(converted);
    EXPECT_NEAR(converted, original, 1e-6);
}

TEST(ColorTest, ConvertToLinearAndBack) {
    double original = 0.5;
    double converted = original;
    convertToLinearSpace(converted);
    convertToSRGBSpace(converted);
    EXPECT_NEAR(converted, original, 1e-6);
}

TEST(ColorTest, ConvertToLinearFloatBelowThreshold) {
    float value = 0.02f;
    float expected = value * static_cast<float>(SRGB_THRESHOLD_FACTOR);
    convertToLinearSpace(value);
    EXPECT_NEAR(value, expected, 1e-7f);
}

TEST(ColorTest, ConvertToLinearFloatAboveThreshold) {
    float value = 0.5f;
    float expected = std::pow((value + static_cast<float>(COLOR_SPACE_ADDENDUM)) * static_cast<float>(SRGB_FACTOR),
                              static_cast<float>(SRGB_GAMMA));
    convertToLinearSpace(value);
    EXPECT_NEAR(value, expected, 1e-7f);
}

TEST(ColorTest, RGBALinearSpaceBackSRGB) {
  ColorRGBA color(0.3, 0.5, 0.7, 0.8);
  ColorRGBA linear_color = color.toLinearSpace();
  ColorRGBA srgb_color   = linear_color.toSRGBSpace();
  EXPECT_NEAR(srgb_color.r, color.r, 1e-6);
  EXPECT_NEAR(srgb_color.g, color.g, 1e-6);
  EXPECT_NEAR(srgb_color.b, color.b, 1e-6);
  EXPECT_NEAR(srgb_color.a, color.a, 1e-6);
}

TEST(ColorTest, RGBStreamOperator) {
  ColorRGB color{0.1, 0.2, 0.3};

  std::ostringstream oss;
  oss << color;

  EXPECT_EQ(oss.str(), "ColorRGB(0.1, 0.2, 0.3)");
}

TEST(ColorTest, RGBAStreamOperator) {
  ColorRGBA color{0.1, 0.2, 0.3, 0.4};

  std::ostringstream oss;
  oss << color;

  EXPECT_EQ(oss.str(), "ColorRGBA(0.1, 0.2, 0.3, 0.4)");
}
