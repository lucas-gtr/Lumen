#include "Core/CommonTypes.hpp"
#include <gtest/gtest.h>

TEST(ColorRGBTest, DefaultConstructor) {
  ColorRGB color;
  EXPECT_DOUBLE_EQ(color.r, 0.0);
  EXPECT_DOUBLE_EQ(color.g, 0.0);
  EXPECT_DOUBLE_EQ(color.b, 0.0);
}

TEST(ColorRGBTest, RGBConstructor) {
  ColorRGB color(0.1, 0.5, 0.9);
  EXPECT_DOUBLE_EQ(color.r, 0.1);
  EXPECT_DOUBLE_EQ(color.g, 0.5);
  EXPECT_DOUBLE_EQ(color.b, 0.9);
}

TEST(ColorRGBTest, GrayscaleConstructor) {
  ColorRGB color(0.7);
  EXPECT_DOUBLE_EQ(color.r, 0.7);
  EXPECT_DOUBLE_EQ(color.g, 0.7);
  EXPECT_DOUBLE_EQ(color.b, 0.7);
}

TEST(ColorRGBTest, AdditionOperator) {
  ColorRGB c1(0.2, 0.3, 0.4);
  ColorRGB c2(0.1, 0.2, 0.3);
  ColorRGB result = c1 + c2;
  EXPECT_DOUBLE_EQ(result.r, 0.3);
  EXPECT_DOUBLE_EQ(result.g, 0.5);
  EXPECT_DOUBLE_EQ(result.b, 0.7);
}

TEST(ColorRGBTest, AdditionAssignmentOperator) {
  ColorRGB c1(0.2, 0.3, 0.4);
  ColorRGB c2(0.1, 0.2, 0.3);
  c1 += c2;
  EXPECT_DOUBLE_EQ(c1.r, 0.3);
  EXPECT_DOUBLE_EQ(c1.g, 0.5);
  EXPECT_DOUBLE_EQ(c1.b, 0.7);
}

TEST(ColorRGBTest, ScalarMultiplicationOperator) {
  ColorRGB color(0.2, 0.3, 0.4);
  ColorRGB result = color * 2.0;
  EXPECT_DOUBLE_EQ(result.r, 0.4);
  EXPECT_DOUBLE_EQ(result.g, 0.6);
  EXPECT_DOUBLE_EQ(result.b, 0.8);
}

TEST(ColorRGBTest, ScalarMultiplicationFriendOperator) {
  ColorRGB color(0.2, 0.3, 0.4);
  ColorRGB result = 2.0 * color;
  EXPECT_DOUBLE_EQ(result.r, 0.4);
  EXPECT_DOUBLE_EQ(result.g, 0.6);
  EXPECT_DOUBLE_EQ(result.b, 0.8);
}

TEST(ColorRGBTest, EqualityOperator) {
  ColorRGB c1(0.1, 0.2, 0.3);
  ColorRGB c2(0.1, 0.2, 0.3);
  ColorRGB c3(0.3, 0.2, 0.1);
  EXPECT_TRUE(c1 == c2);
  EXPECT_FALSE(c1 == c3);
}

TEST(ColorRGBTest, StreamOutput) {
  ColorRGB color(0.5, 0.6, 0.7);
  std::ostringstream oss;
  oss << color;
  EXPECT_EQ(oss.str(), "ColorRGB(0.5, 0.6, 0.7)");
}

TEST(ColorRGBATest, DefaultConstructor) {
  ColorRGBA color;
  EXPECT_DOUBLE_EQ(color.r, 0.0);
  EXPECT_DOUBLE_EQ(color.g, 0.0);
  EXPECT_DOUBLE_EQ(color.b, 0.0);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(ColorRGBATest, RGBAConstructor) {
  ColorRGBA color(0.2, 0.4, 0.6, 0.8);
  EXPECT_DOUBLE_EQ(color.r, 0.2);
  EXPECT_DOUBLE_EQ(color.g, 0.4);
  EXPECT_DOUBLE_EQ(color.b, 0.6);
  EXPECT_DOUBLE_EQ(color.a, 0.8);
}

TEST(ColorRGBATest, GrayscaleConstructor) {
  ColorRGBA color(0.5);
  EXPECT_DOUBLE_EQ(color.r, 0.5);
  EXPECT_DOUBLE_EQ(color.g, 0.5);
  EXPECT_DOUBLE_EQ(color.b, 0.5);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(ColorRGBATest, GrayscaleAlphaConstructor) {
  ColorRGBA color(0.6, 0.3);
  EXPECT_DOUBLE_EQ(color.r, 0.6);
  EXPECT_DOUBLE_EQ(color.g, 0.6);
  EXPECT_DOUBLE_EQ(color.b, 0.6);
  EXPECT_DOUBLE_EQ(color.a, 0.3);
}

TEST(ColorRGBATest, RGBConversionConstructor) {
  ColorRGB rgb(0.1, 0.2, 0.3);
  ColorRGBA color(rgb);
  EXPECT_DOUBLE_EQ(color.r, 0.1);
  EXPECT_DOUBLE_EQ(color.g, 0.2);
  EXPECT_DOUBLE_EQ(color.b, 0.3);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(ColorRGBATest, RGBConversionWithAlphaConstructor) {
  ColorRGB rgb(0.4, 0.5, 0.6);
  ColorRGBA color(rgb, 0.7);
  EXPECT_DOUBLE_EQ(color.r, 0.4);
  EXPECT_DOUBLE_EQ(color.g, 0.5);
  EXPECT_DOUBLE_EQ(color.b, 0.6);
  EXPECT_DOUBLE_EQ(color.a, 0.7);
}

TEST(ColorRGBATest, AdditionOperator) {
  ColorRGBA c1(0.1, 0.2, 0.3, 0.4);
  ColorRGBA c2(0.4, 0.3, 0.2, 0.1);
  ColorRGBA result = c1 + c2;
  EXPECT_DOUBLE_EQ(result.r, 0.5);
  EXPECT_DOUBLE_EQ(result.g, 0.5);
  EXPECT_DOUBLE_EQ(result.b, 0.5);
  EXPECT_DOUBLE_EQ(result.a, 0.5);
}

TEST(ColorRGBATest, ScalarMultiplicationOperator) {
  ColorRGBA color(0.1, 0.2, 0.3, 0.4);
  ColorRGBA result = color * 2.0;
  EXPECT_DOUBLE_EQ(result.r, 0.2);
  EXPECT_DOUBLE_EQ(result.g, 0.4);
  EXPECT_DOUBLE_EQ(result.b, 0.6);
  EXPECT_DOUBLE_EQ(result.a, 0.8);
}

TEST(ColorRGBATest, ScalarMultiplicationFriendOperator) {
  ColorRGBA color(0.1, 0.2, 0.3, 0.4);
  ColorRGBA result = 2.0 * color;
  EXPECT_DOUBLE_EQ(result.r, 0.2);
  EXPECT_DOUBLE_EQ(result.g, 0.4);
  EXPECT_DOUBLE_EQ(result.b, 0.6);
  EXPECT_DOUBLE_EQ(result.a, 0.8);
}

TEST(ColorRGBATest, EqualityOperator) {
  ColorRGBA c1(0.1, 0.2, 0.3, 0.4);
  ColorRGBA c2(0.1, 0.2, 0.3, 0.4);
  ColorRGBA c3(0.1, 0.2, 0.3, 1.0);
  EXPECT_TRUE(c1 == c2);
  EXPECT_FALSE(c1 == c3);
}

TEST(ColorRGBATest, StreamOutput) {
  ColorRGBA color(0.5, 0.6, 0.7, 0.8);
  std::ostringstream oss;
  oss << color;
  EXPECT_EQ(oss.str(), "ColorRGBA(0.5, 0.6, 0.7, 0.8)");
}