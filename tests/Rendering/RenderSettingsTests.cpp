#include "Rendering/RenderSettings.hpp"

#include <gtest/gtest.h>

TEST(RenderSettingsTest, DefaultValues) {
  RenderSettings settings;

  EXPECT_EQ(settings.getWidth(), 800);
  EXPECT_EQ(settings.getHeight(), 800);
  EXPECT_EQ(settings.getChannelCount(), 3);
  EXPECT_EQ(settings.getMaxBounces(), 5);
  EXPECT_EQ(settings.getSamplesPerPixel(), 9);
}

TEST(RenderSettingsTest, SetAndGetWidth) {
  RenderSettings settings;
  settings.setWidth(1024);
  EXPECT_EQ(settings.getWidth(), 1024);
}

TEST(RenderSettingsTest, SetAndGetHeight) {
  RenderSettings settings;
  settings.setHeight(768);
  EXPECT_EQ(settings.getHeight(), 768);
}

TEST(RenderSettingsTest, SetAndGetChannelCount) {
  RenderSettings settings;
  settings.setChannelCount(4);
  EXPECT_EQ(settings.getChannelCount(), 4);
}

TEST(RenderSettingsTest, SetAndGetMaxBounces) {
  RenderSettings settings;
  settings.setMaxBounces(10);
  EXPECT_EQ(settings.getMaxBounces(), 10);
}

TEST(RenderSettingsTest, SetAndGetSamplesPerPixel) {
  RenderSettings settings;
  settings.setSamplesPerPixel(16);
  EXPECT_EQ(settings.getSamplesPerPixel(), 16);
}

TEST(RenderSettingsTest, SamplesPerPixels) {
  RenderSettings settings;

  settings.setSamplesPerPixel(0);
  EXPECT_EQ(settings.getSamplesPerPixel(), 1);

  settings.setSamplesPerPixel(6);
  EXPECT_EQ(settings.getSamplesPerPixel(), 4);

  settings.setSamplesPerPixel(7);
  EXPECT_EQ(settings.getSamplesPerPixel(), 9);

  settings.setSamplesPerPixel(9);
  EXPECT_EQ(settings.getSamplesPerPixel(), 9);
}

TEST(RendererSettingsTest, DefaultExecutionModeIsSingleThreaded) {
  RenderSettings settings;

  EXPECT_EQ(settings.getRenderMode(), RenderMode::SINGLE_THREADED);
}

TEST(RendererSettingsTest, SetAndGetExecutionMode) {
  RenderSettings settings;

  settings.setRenderMode(RenderMode::MULTI_THREADED_CPU);
  EXPECT_EQ(settings.getRenderMode(), RenderMode::MULTI_THREADED_CPU);

  settings.setRenderMode(RenderMode::SINGLE_THREADED);
  EXPECT_EQ(settings.getRenderMode(), RenderMode::SINGLE_THREADED);
}

TEST(RendererSettingsTest, SetAndGetChunkSize) {
  RenderSettings settings;

  settings.setChunkSize(64);
  EXPECT_EQ(settings.getChunkSize(), 64);
}

TEST(RendererSettingsTest, SetAndGetThreadCount) {
  RenderSettings settings;

  settings.setThreadCount(8);

  unsigned int max_thread = std::max(1U, std::thread::hardware_concurrency() - 4);

  EXPECT_EQ(settings.getThreadCount(), std::min(8U, max_thread));

  unsigned int hardware_concurrency = std::thread::hardware_concurrency();
  settings.setThreadCount(hardware_concurrency + 2);
  EXPECT_EQ(settings.getThreadCount(), std::max(1U, hardware_concurrency - 4));
}