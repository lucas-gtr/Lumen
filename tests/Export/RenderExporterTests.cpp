#include "Export/RenderExporter.hpp"
#include "Export/OutputFormatPng.hpp"
#include "PostProcessing/ToneMapping/Exposure.hpp"
#include "PostProcessing/ToneMapping/None.hpp"
#include "PostProcessing/ToneMapping/LuminanceReinhard.hpp"
#include "Core/Framebuffer.hpp"

#include <gtest/gtest.h>
#include <fstream>
#include <utility>

TEST(RenderExporterTest, GettersCorrect) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);
    exporter.setPath("RenderImages/");
    exporter.setFilename("image");

    EXPECT_EQ(exporter.getFilename(), "image");
    EXPECT_EQ(exporter.getPath(), "RenderImages/");

    exporter.setPath("RenderImages");
    EXPECT_EQ(exporter.getPath(), "RenderImages/");
}

TEST(RenderExporterTest, ExportRenderPng) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setOutputFormat(OutputFormat::PNG);
    exporter.setToneMapping(ToneMapping::LUMINANCE_REINHARD);
    exporter.setPath("/tmp");
    exporter.setFilename("test_image.png");

    EXPECT_EQ(exporter.getFramebuffer(), &framebuffer);
    exporter.exportRender();

    std::ifstream file("/tmp/test_image.png");
    ASSERT_TRUE(file.good());
    file.close();

    std::remove("/tmp/test_image.png");
}

TEST(RenderExporterTest, ExportRenderHdr) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setOutputFormat(OutputFormat::HDR);
    exporter.setToneMapping(ToneMapping::EXPOSURE);
    exporter.setPath("/tmp");
    exporter.setFilename("test_image.hdr");

    EXPECT_EQ(exporter.getFramebuffer(), &framebuffer);
    exporter.exportRender();

    std::ifstream file("/tmp/test_image.hdr");
    ASSERT_TRUE(file.good());
    file.close();

    std::remove("/tmp/test_image.hdr");
}

TEST(RenderExporterTest, ToneMappingParameters) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);
    
    exporter.setToneMapping(ToneMapping::EXPOSURE);

    EXPECT_DOUBLE_EQ(exporter.getExposure(), 1.0);

    exporter.setExposure(2.0);

    EXPECT_DOUBLE_EQ(exporter.getExposure(), 2.0);
}

TEST(RenderExporterTest, SetAndGetChannelCount) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setChannelCount(3);
    EXPECT_EQ(exporter.getChannelCount(), 3);

    exporter.setChannelCount(4);
    EXPECT_EQ(exporter.getChannelCount(), 4);

    exporter.setChannelCount(1);
    EXPECT_EQ(exporter.getChannelCount(), 1);

    EXPECT_THROW(exporter.setChannelCount(2), std::invalid_argument);
}

TEST(RenderExporterTest, SetAndGetToneMapping) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setToneMapping(ToneMapping::NONE);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::NONE);

    exporter.setToneMapping(ToneMapping::LUMINANCE_REINHARD);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::LUMINANCE_REINHARD);

    exporter.setToneMapping(ToneMapping::EXPOSURE);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::EXPOSURE);

    exporter.setToneMapping(ToneMapping::WHITE_POINT_REINHARD);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::WHITE_POINT_REINHARD);

    exporter.setToneMapping(ToneMapping::UNCHARTED2);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::UNCHARTED2);

    exporter.setToneMapping(ToneMapping::ACES);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::ACES);
}

TEST(RenderExporterTest, SetAndGetWhitePoint) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setWhitePoint(1.0);
    EXPECT_DOUBLE_EQ(exporter.getWhitePoint(), 1.0);

    exporter.setWhitePoint(2.0);
    EXPECT_DOUBLE_EQ(exporter.getWhitePoint(), 2.0);

    exporter.setWhitePoint(0.5);
    EXPECT_DOUBLE_EQ(exporter.getWhitePoint(), 0.5);
}

TEST(RenderExporterTest, SetAndGetOutputFormat) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setOutputFormat(OutputFormat::JPEG);
    EXPECT_EQ(exporter.getOutputFormat(), OutputFormat::JPEG);

    exporter.setOutputFormat(OutputFormat::BMP);
    EXPECT_EQ(exporter.getOutputFormat(), OutputFormat::BMP);

    exporter.setOutputFormat(OutputFormat::TGA);
    EXPECT_EQ(exporter.getOutputFormat(), OutputFormat::TGA);

    exporter.setOutputFormat(OutputFormat::HDR);
    EXPECT_EQ(exporter.getOutputFormat(), OutputFormat::HDR);
}

TEST(RenderExporterTest, SetAndGetJpegQuality) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.setOutputFormat(OutputFormat::JPEG);
    exporter.setJpegQuality(75);
    EXPECT_EQ(exporter.getJpegQuality(), 75);

    exporter.setJpegQuality(50);
    EXPECT_EQ(exporter.getJpegQuality(), 50);

    exporter.setJpegQuality(101);
    EXPECT_EQ(exporter.getJpegQuality(), 100);

    exporter.setJpegQuality(-1);
    EXPECT_EQ(exporter.getJpegQuality(), 1);
}

TEST(RenderExporterTest, UpdateImageToExport) {
    Framebuffer framebuffer({800, 600});
    RenderExporter exporter(&framebuffer);

    exporter.updateImageToExport();
    const unsigned char* image_data = exporter.getImageToExport();

    EXPECT_NE(image_data, nullptr);
    EXPECT_EQ(exporter.getFramebuffer()->getWidth(), 800);
    EXPECT_EQ(exporter.getFramebuffer()->getHeight(), 600);
}

TEST(RenderExporterTest, ExposureChangesToneMappingOutput) {
  Framebuffer framebuffer({2, 2});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  RenderExporter exporter(&framebuffer);
  exporter.setToneMapping(ToneMapping::EXPOSURE);

  PixelCoord pixel{1, 1};
  ColorRGB color{0.5, 0.5, 0.5};
  framebuffer.setPixelColor(pixel, color, 1.0);
  framebuffer.reduceThreadBuffers();

  exporter.updateImageToExport();
  const unsigned char* imageBefore = exporter.getImageToExport();

  int idx = (1 * 2 + 1) * 3;

  unsigned char rBefore = imageBefore[idx];
  unsigned char gBefore = imageBefore[idx + 1];
  unsigned char bBefore = imageBefore[idx + 2];

  exporter.setExposure(2.0);
  exporter.updateImageToExport();
  const unsigned char* imageAfter = exporter.getImageToExport();

  unsigned char rAfter = imageAfter[idx];
  unsigned char gAfter = imageAfter[idx + 1];
  unsigned char bAfter = imageAfter[idx + 2];

  EXPECT_GT(rAfter, rBefore);
  EXPECT_GT(gAfter, gBefore);
  EXPECT_GT(bAfter, bBefore);
}

TEST(RenderExporterTest, WhitePointChangesToneMappingOutput) {
  Framebuffer framebuffer({2, 2});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  RenderExporter exporter(&framebuffer);

  exporter.setToneMapping(ToneMapping::WHITE_POINT_REINHARD);

  PixelCoord pixel{1, 1};
  ColorRGB color{0.5, 0.5, 0.5};
  framebuffer.setPixelColor(pixel, color, 1.0);
  framebuffer.reduceThreadBuffers();

  exporter.updateImageToExport();
  const unsigned char* imageBefore = exporter.getImageToExport();

  int idx = (1 * 2 + 1) * 3;

  unsigned char rBefore = imageBefore[idx];
  unsigned char gBefore = imageBefore[idx + 1];
  unsigned char bBefore = imageBefore[idx + 2];

  exporter.setWhitePoint(2.0);
  exporter.updateImageToExport();
  const unsigned char* imageAfter = exporter.getImageToExport();

  unsigned char rAfter = imageAfter[idx];
  unsigned char gAfter = imageAfter[idx + 1];
  unsigned char bAfter = imageAfter[idx + 2];

  EXPECT_GT(rAfter, rBefore);
  EXPECT_GT(gAfter, gBefore);
  EXPECT_GT(bAfter, bBefore);
}

TEST(RenderExporterTest, Uncharted2ExposureAndWhitePointChangeOutput) {
  Framebuffer framebuffer({2, 2});
  framebuffer.initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  RenderExporter exporter(&framebuffer);
  exporter.setToneMapping(ToneMapping::UNCHARTED2);

  PixelCoord pixel{1, 1};
  ColorRGB color{0.5, 0.5, 0.5};
  framebuffer.setPixelColor(pixel, color, 1.0);
  framebuffer.reduceThreadBuffers();

  exporter.updateImageToExport();
  const unsigned char* imageInitial = exporter.getImageToExport();

  int idx = (pixel.y * 2 + pixel.x) * 3;

  unsigned char rInitial = imageInitial[idx];
  unsigned char gInitial = imageInitial[idx + 1];
  unsigned char bInitial = imageInitial[idx + 2];

  exporter.setExposure(2.0);
  exporter.updateImageToExport();
  const unsigned char* imageAfterExposure = exporter.getImageToExport();

  unsigned char rAfterExposure = imageAfterExposure[idx];
  unsigned char gAfterExposure = imageAfterExposure[idx + 1];
  unsigned char bAfterExposure = imageAfterExposure[idx + 2];

  EXPECT_GT(rAfterExposure, rInitial);
  EXPECT_GT(gAfterExposure, gInitial);
  EXPECT_GT(bAfterExposure, bInitial);

  exporter.setWhitePoint(2.0);
  exporter.updateImageToExport();
  const unsigned char* imageAfterWhitePoint = exporter.getImageToExport();

  unsigned char rAfterWhitePoint = imageAfterWhitePoint[idx];
  unsigned char gAfterWhitePoint = imageAfterWhitePoint[idx + 1];
  unsigned char bAfterWhitePoint = imageAfterWhitePoint[idx + 2];

  EXPECT_GT(rAfterWhitePoint, rAfterExposure);
  EXPECT_GT(gAfterWhitePoint, gAfterExposure);
  EXPECT_GT(bAfterWhitePoint, bAfterExposure);
}