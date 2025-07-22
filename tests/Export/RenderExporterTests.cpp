#include "Export/RenderExporter.hpp"
#include "Export/OutputFormatPng.hpp"
#include "PostProcessing/ToneMapping/Exposure.hpp"
#include "PostProcessing/ToneMapping/None.hpp"
#include "PostProcessing/ToneMapping/Reinhard.hpp"

#include <gtest/gtest.h>
#include <fstream>
#include <utility>

TEST(RenderExporterTest, GettersCorrect) {
    Framebuffer framebuffer({800, 600, 1});
    RenderExporter exporter(&framebuffer);
    exporter.setPath("RenderImages/");
    exporter.setFilename("image");

    EXPECT_EQ(exporter.getFilename(), "image");
    EXPECT_EQ(exporter.getPath(), "RenderImages/");

    exporter.setPath("RenderImages");
    EXPECT_EQ(exporter.getPath(), "RenderImages/");
}

TEST(RenderExporterTest, ExportRenderPng) {
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);

    exporter.setOutputFormat(OutputFormat::PNG);
    exporter.setToneMapping(ToneMapping::REINHARD);
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
    Framebuffer framebuffer({800, 600, 4});
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
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);
    
    exporter.setToneMapping(ToneMapping::EXPOSURE);

    EXPECT_DOUBLE_EQ(exporter.getExposure(), 1.0);

    exporter.setExposure(2.0);

    EXPECT_DOUBLE_EQ(exporter.getExposure(), 2.0);
}

TEST(RenderExporterTest, SetAndGetToneMapping) {
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);

    exporter.setToneMapping(ToneMapping::NONE);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::NONE);

    exporter.setToneMapping(ToneMapping::REINHARD);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::REINHARD);

    exporter.setToneMapping(ToneMapping::EXPOSURE);
    EXPECT_EQ(exporter.getToneMapping(), ToneMapping::EXPOSURE);
}

TEST(RenderExporterTest, SetAndGetOutputFormat) {
    Framebuffer framebuffer({800, 600, 3});
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
    Framebuffer framebuffer({800, 600, 3});
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
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);

    exporter.updateImageToExport();
    const unsigned char* image_data = exporter.getImageToExport();

    EXPECT_NE(image_data, nullptr);
    EXPECT_EQ(exporter.getFramebuffer()->getWidth(), 800);
    EXPECT_EQ(exporter.getFramebuffer()->getHeight(), 600);
    EXPECT_EQ(exporter.getFramebuffer()->getChannelCount(), 3);
}