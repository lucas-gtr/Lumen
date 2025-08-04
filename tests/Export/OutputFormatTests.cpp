#include <gtest/gtest.h>
#include "Export/OutputFormat.hpp"

TEST(OutputFormatTest, StringToOutputFormatMap) {
    const auto& format_map = stringToOutputFormatMap();

    EXPECT_EQ(format_map.at("JPEG"), OutputFormat::JPEG);
    EXPECT_EQ(format_map.at("BMP"), OutputFormat::BMP);
    EXPECT_EQ(format_map.at("TGA"), OutputFormat::TGA);
    EXPECT_EQ(format_map.at("HDR"), OutputFormat::HDR);
    EXPECT_EQ(format_map.at("PNG"), OutputFormat::PNG);
}

TEST(OutputFormatTest, OutputFormatToString) {
    EXPECT_EQ(outputFormatToString(OutputFormat::PNG), "PNG");
    EXPECT_EQ(outputFormatToString(OutputFormat::JPEG), "JPEG");
    EXPECT_EQ(outputFormatToString(OutputFormat::BMP), "BMP");
    EXPECT_EQ(outputFormatToString(OutputFormat::TGA), "TGA");
    EXPECT_EQ(outputFormatToString(OutputFormat::HDR), "HDR");
    EXPECT_EQ(outputFormatToString(static_cast<OutputFormat>(999)), "PNG");
}

TEST(OutputFormatTest, StringToOutputFormat) {
    EXPECT_EQ(stringToOutputFormat("PNG"), OutputFormat::PNG);
    EXPECT_EQ(stringToOutputFormat("JPEG"), OutputFormat::JPEG);
    EXPECT_EQ(stringToOutputFormat("BMP"), OutputFormat::BMP);
    EXPECT_EQ(stringToOutputFormat("TGA"), OutputFormat::TGA);
    EXPECT_EQ(stringToOutputFormat("HDR"), OutputFormat::HDR);
    EXPECT_EQ(stringToOutputFormat("UNKNOWN"), OutputFormat::PNG);
}