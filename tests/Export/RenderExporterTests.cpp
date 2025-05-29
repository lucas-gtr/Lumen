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
    std::unique_ptr<OutputFormat> png_format = std::make_unique<OutputFormatPng>();

    exporter.setOutputFormat(std::move(png_format));
    exporter.setToneMapping(ToneMapping::REINHARD);
    exporter.setPath("/tmp");
    exporter.setFilename("test_image");

    EXPECT_EQ(exporter.getFramebuffer(), &framebuffer);
    exporter.exportRender();

    std::ifstream file("/tmp/test_image.png");
    ASSERT_TRUE(file.good());
    file.close();

    std::remove("/tmp/test_image.png");
}

TEST(RenderExporterTest, NoExportOutputFormatNull) {
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);
    
    exporter.setToneMapping(ToneMapping::NONE);
    exporter.setPath("/tmp");
    exporter.setFilename("test_image");

    exporter.exportRender();

    std::ifstream file("/tmp/test_image.png");
    ASSERT_FALSE(file.good());
    file.close();
}

TEST(RenderExporterTest, ExportToneMappingNull) {
    Framebuffer framebuffer({800, 600, 4});
    RenderExporter exporter(&framebuffer);
    std::unique_ptr<OutputFormat> png_format = std::make_unique<OutputFormatPng>();

    exporter.setOutputFormat(std::move(png_format));
    exporter.setPath("/tmp");
    exporter.setFilename("test_image");

    exporter.exportRender();

    std::ifstream file("/tmp/test_image.png");
    ASSERT_TRUE(file.good());
    file.close();

    std::remove("/tmp/test_image.png");
}

TEST(RenderExporterTest, ToneMappingParameters) {
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);
    
    exporter.setToneMapping(ToneMapping::EXPOSURE);

    EXPECT_EQ(exporter.getParameters()->get<DoubleParam>("exposure")->getValue(), 1.0);

    exporter.setParameter("exposure", 2.0);

    EXPECT_EQ(exporter.getParameters()->get<DoubleParam>("exposure")->getValue(), 2.0);
}