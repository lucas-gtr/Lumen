#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include "Export/RenderExporter.hpp"
#include "Export/OutputFormatPng.hpp"

TEST(RenderExporterTest, GettersCorrect) {
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);
    exporter.setPath("RenderImages/");
    exporter.setFilename("image");

    EXPECT_EQ(exporter.getFilename(), "image");
    EXPECT_EQ(exporter.getPath(), "RenderImages/");

    exporter.setPath("RenderImages");
    EXPECT_EQ(exporter.getPath(), "RenderImages/");
}

TEST(RenderExporterTest, ExportRender) {
    Framebuffer framebuffer({800, 600, 3});
    RenderExporter exporter(&framebuffer);
    OutputFormatPng output_format;
    exporter.setOutputFormat(&output_format);
    exporter.setPath("/tmp");
    exporter.setFilename("test_image");

    EXPECT_EQ(exporter.getFramebuffer(), &framebuffer);
    exporter.exportRender();

    std::ifstream file("/tmp/test_image.png");
    ASSERT_TRUE(file.good());
    file.close();

    std::remove("/tmp/test_image.png");
}