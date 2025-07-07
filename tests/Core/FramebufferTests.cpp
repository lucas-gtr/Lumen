#include <gtest/gtest.h>

#include "Core/ColorUtils.hpp"
#include "Core/Framebuffer.hpp"

class FramebufferTest : public ::testing::Test {
protected:
  ImageProperties default_props{4, 4, 3}; 
  Framebuffer framebuffer{default_props};

  PixelCoord pixel00{0, 0};
  PixelCoord pixel22{2, 2};

  ColorRGBA red{1.0, 0.0, 0.0, 1.0};
  ColorRGBA green{0.0, 1.0, 0.0, 1.0};
  ColorRGBA blue{0.0, 0.0, 1.0, 1.0};

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

TEST_F(FramebufferTest, SetFramebufferPropertiesReallocates) {
  ImageProperties new_props{2, 2, 1};
  framebuffer.setFramebufferProperties(new_props);
  EXPECT_EQ(framebuffer.getWidth(), 2);
  EXPECT_EQ(framebuffer.getHeight(), 2);
  EXPECT_EQ(framebuffer.getChannelCount(), 1);
  EXPECT_EQ(framebuffer.getSize(), 4);
}

TEST_F(FramebufferTest, SetFramebufferPropertiesReallocatesChannels) {
  ImageProperties new_props{4, 4, 4};
  framebuffer.setFramebufferProperties(new_props);
  EXPECT_EQ(framebuffer.getWidth(), 4);
  EXPECT_EQ(framebuffer.getHeight(), 4);
  EXPECT_EQ(framebuffer.getChannelCount(), 4);
  EXPECT_EQ(framebuffer.getSize(), 64);
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
  framebuffer.setPixelColor(pixel00, ColorRGBA(0.5), 1.0);
  framebuffer.reduceThreadBuffers();
  framebuffer.convertToSRGBColorSpace();
  double expected = 0.5;
  convertToSRGBSpace(expected);

  EXPECT_DOUBLE_EQ(framebuffer.getFramebuffer()[0], expected);
  EXPECT_DOUBLE_EQ(framebuffer.getFramebuffer()[1], expected);
  EXPECT_DOUBLE_EQ(framebuffer.getFramebuffer()[2], expected);
}

TEST_F(FramebufferTest, GrayscalePixelSetCorrectly) {
  framebuffer.setFramebufferProperties({2, 2, 1});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  framebuffer.setPixelColor({1, 1}, ColorRGBA{0.2, 0.4, 0.6, 1.0}, 1.0);
  framebuffer.reduceThreadBuffers();

  const int index = (1 * 2 + 1) * 1;
  const double* data = framebuffer.getFramebuffer();
  const double expected = toGrayscale(ColorRGBA{0.2, 0.4, 0.6, 1.0});
  EXPECT_NEAR(data[index], expected, 1e-8);
}

TEST_F(FramebufferTest, AlphaChannelIsAddedWhenChannelsEqual4) {
  framebuffer.setFramebufferProperties({1, 1, 4});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  framebuffer.setPixelColor({0, 0}, ColorRGBA{0.5, 0.6, 0.7, 1.0}, 1.0);
  framebuffer.reduceThreadBuffers();

  const double* data = framebuffer.getFramebuffer();
  EXPECT_DOUBLE_EQ(data[0], 0.5); 
  EXPECT_DOUBLE_EQ(data[1], 0.6); 
  EXPECT_DOUBLE_EQ(data[2], 0.7); 
  EXPECT_DOUBLE_EQ(data[3], 1.0); 
}
TEST_F(FramebufferTest, UnsupportedChannelCountDoesNothing) {
  framebuffer.setFramebufferProperties({2, 2, 2});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  testing::internal::CaptureStderr();
  framebuffer.setPixelColor({0, 0}, ColorRGBA{0.5, 0.6, 0.7, 1.0}, 1.0);
  std::string output = testing::internal::GetCapturedStderr();

  const double* data = framebuffer.getFramebuffer();
  for (size_t i = 0; i < framebuffer.getSize(); ++i) {
    EXPECT_DOUBLE_EQ(data[i], 0.0);
  }

  EXPECT_TRUE(output.find("Unsupported channel count") != std::string::npos);
}

TEST_F(FramebufferTest, InvalidThreadIdThrowsError) {
  testing::internal::CaptureStderr();
  
  Framebuffer::SetThreadId(-1);
  framebuffer.setPixelColor({0, 0}, ColorRGBA{0.5, 0.6, 0.7, 1.0}, 1.0); 
  std::string outputNegative = testing::internal::GetCapturedStderr();
  EXPECT_TRUE(outputNegative.find("Invalid thread ID") != std::string::npos);

  testing::internal::CaptureStderr();
  
  Framebuffer::SetThreadId(99); 
  framebuffer.setPixelColor({0, 0}, ColorRGBA{0.5, 0.6, 0.7, 1.0}, 1.0); 
  std::string outputExceeding = testing::internal::GetCapturedStderr();
  EXPECT_TRUE(outputExceeding.find("Invalid thread ID") != std::string::npos);
}


