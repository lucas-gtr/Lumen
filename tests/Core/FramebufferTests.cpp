#include <gtest/gtest.h>

#include "Core/Framebuffer.hpp"

class FramebufferTest : public ::testing::Test {
protected:
  Resolution default_res{4, 4}; 
  Framebuffer framebuffer{default_res};

  PixelCoord pixel00{0, 0};
  PixelCoord pixel22{2, 2};

  ColorRGB red{1.0, 0.0, 0.0};
  ColorRGB green{0.0, 1.0, 0.0};
  ColorRGB blue{0.0, 0.0, 1.0};

  void SetUp() override {
    framebuffer.initThreadBuffers(2);
    Framebuffer::SetThreadId(0);
  }
};

TEST_F(FramebufferTest, ConstructorInitializesFramebuffer) {
  EXPECT_EQ(framebuffer.getWidth(), 4);
  EXPECT_EQ(framebuffer.getHeight(), 4);
  EXPECT_EQ(framebuffer.getChannelCount(), 3);
  EXPECT_EQ(framebuffer.getSize(), 48);
  const double* data = framebuffer.getFramebuffer();
  for (size_t i = 0; i < framebuffer.getSize(); ++i) {
    EXPECT_DOUBLE_EQ(data[i], 0.0);
  }
}

TEST_F(FramebufferTest, InitThreadBuffersAllocatesCorrectSize) {
  framebuffer.initThreadBuffers(3);
  Framebuffer::SetThreadId(2);
  framebuffer.setPixelColor(pixel00, red, 1.0);
}

TEST_F(FramebufferTest, SetPixelColorRGBThreadBufferUpdated) {
  framebuffer.setPixelColor(pixel22, red, 1.0);
  framebuffer.reduceThreadBuffers();

  const int idx = (2 * 4 + 2) * 3;
  const double* data = framebuffer.getFramebuffer();
  EXPECT_DOUBLE_EQ(data[idx + 0], 1.0);
  EXPECT_DOUBLE_EQ(data[idx + 1], 0.0);
  EXPECT_DOUBLE_EQ(data[idx + 2], 0.0);
}

TEST_F(FramebufferTest, SetPixelColorOutOfBoundsIsIgnored) {
  PixelCoord invalid{-1, 10};
  testing::internal::CaptureStderr();
  framebuffer.setPixelColor(invalid, red, 1.0);
  std::string output = testing::internal::GetCapturedStderr();
  EXPECT_TRUE(output.find("Pixel coordinates out of bounds") != std::string::npos);
}

TEST_F(FramebufferTest, ReduceThreadBuffersCombinesData) {
  Framebuffer::SetThreadId(0);
  framebuffer.setPixelColor(pixel22, red, 0.5);

  Framebuffer::SetThreadId(1);
  framebuffer.setPixelColor(pixel22, red, 0.5);

  framebuffer.reduceThreadBuffers();
  const int idx = (2 * 4 + 2) * 3;
  const double* data = framebuffer.getFramebuffer();
  EXPECT_DOUBLE_EQ(data[idx], 1.0);
  EXPECT_DOUBLE_EQ(data[idx + 1], 0.0);
  EXPECT_DOUBLE_EQ(data[idx + 2], 0.0);
}

TEST_F(FramebufferTest, ConvertToSRGBColorSpaceDoesNotCrash) {
  framebuffer.setPixelColor(pixel00, ColorRGB(0.5), 1.0);
  framebuffer.reduceThreadBuffers();
  framebuffer.convertToSRGBColorSpace();
  double expected = 0.5;
  convertToSRGBSpace(expected);

  EXPECT_DOUBLE_EQ(framebuffer.getFramebuffer()[0], expected);
  EXPECT_DOUBLE_EQ(framebuffer.getFramebuffer()[1], expected);
  EXPECT_DOUBLE_EQ(framebuffer.getFramebuffer()[2], expected);
}

TEST_F(FramebufferTest, InvalidThreadIdThrowsError) {
  testing::internal::CaptureStderr();
  
  Framebuffer::SetThreadId(-1);
  framebuffer.setPixelColor({0, 0}, ColorRGB{0.5, 0.6, 0.7}, 1.0); 
  std::string outputNegative = testing::internal::GetCapturedStderr();
  EXPECT_TRUE(outputNegative.find("Invalid thread ID") != std::string::npos);

  testing::internal::CaptureStderr();
  
  Framebuffer::SetThreadId(99); 
  framebuffer.setPixelColor({0, 0}, ColorRGB{0.5, 0.6, 0.7}, 1.0); 
  std::string outputExceeding = testing::internal::GetCapturedStderr();
  EXPECT_TRUE(outputExceeding.find("Invalid thread ID") != std::string::npos);
}

TEST_F(FramebufferTest, ScaleBufferValuesScalesCorrectly) {
  framebuffer.setResolution({2, 2});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  framebuffer.setPixelColor({0, 0}, ColorRGB{0.5, 0.5, 0.5}, 1.0);
  framebuffer.reduceThreadBuffers();

  framebuffer.scaleBufferValues(2.0);

  const double* data = framebuffer.getFramebuffer();
  EXPECT_DOUBLE_EQ(data[0], 1.0); 
  EXPECT_DOUBLE_EQ(data[1], 1.0); 
  EXPECT_DOUBLE_EQ(data[2], 1.0); 
}

TEST_F(FramebufferTest, GetMaxValueRGB) {
  framebuffer.setResolution({2, 2});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  framebuffer.setPixelColor({0, 0}, ColorRGB{0.5, 0.7, 0.3}, 1.0);
  framebuffer.reduceThreadBuffers();

  double max_value = framebuffer.getMaximumValue();
  
  EXPECT_DOUBLE_EQ(max_value, 0.7);
}

