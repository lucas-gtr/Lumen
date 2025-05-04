#include "Rendering/RenderSettings.hpp"

#include <gtest/gtest.h>

TEST(RenderSettingsTest, DefaultValues) {
  RenderSettings settings;

  EXPECT_EQ(settings.getWidth(), 800);
  EXPECT_EQ(settings.getHeight(), 600);
  EXPECT_EQ(settings.getChannelCount(), 3);
  EXPECT_DOUBLE_EQ(settings.getNearPlane(), 0.1);
  EXPECT_DOUBLE_EQ(settings.getFarPlane(), 1000.0);
  EXPECT_EQ(settings.getMaxBounces(), 5);
  EXPECT_EQ(settings.getSamplesPerPixel(), 1);
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

TEST(RenderSettingsTest, SetAndGetNearPlane) {
  RenderSettings settings;
  settings.setNearPlane(0.5);
  EXPECT_DOUBLE_EQ(settings.getNearPlane(), 0.5);
}

TEST(RenderSettingsTest, SetAndGetFarPlane) {
  RenderSettings settings;
  settings.setFarPlane(500.0);
  EXPECT_DOUBLE_EQ(settings.getFarPlane(), 500.0);
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

TEST(RenderSettingsTest, NearPlaneCannotBeGreaterThanFarPlane) {
  RenderSettings settings;

  settings.setNearPlane(100.0);
  settings.setFarPlane(50.0);

  EXPECT_LE(settings.getNearPlane(), settings.getFarPlane());
}

TEST(RenderSettingsTest, FarPlaneCannotBeSmallerThanNearPlane) {
  RenderSettings settings;

  settings.setFarPlane(50.0);
  settings.setNearPlane(100.0);

  EXPECT_GE(settings.getFarPlane(), settings.getNearPlane());
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
