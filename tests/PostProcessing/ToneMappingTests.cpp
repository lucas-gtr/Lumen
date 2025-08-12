#include <gtest/gtest.h>
#include "PostProcessing/ToneMapping/Exposure.hpp"
#include "Core/Color.hpp"

TEST(ToneMappingTest, ApplyRGBtoGrayscale) {
  ExposureToneMapping exposure_tm(1.0);

  double framebuffer[] = {
      0.5, 0.25, 0.75, // pixel 1
      1.0, 0.0, 0.0    // pixel 2
  };
  unsigned char output[2];

  exposure_tm.apply(framebuffer, output, 2, 3, 1);

  for (int i = 0; i < 2; ++i) {
      ColorRGB original(framebuffer[i * 3], framebuffer[i * 3 + 1], framebuffer[i * 3 + 2]);
      unsigned char expected_gray = static_cast<unsigned char>(exposure_tm.convertToLDR(original).grayscale() * 255.999);
      EXPECT_EQ(output[i], expected_gray);
  }
}


TEST(ToneMappingTest, ApplyRGB) {
  ExposureToneMapping exposure_tm(1.0);

  double framebuffer[] = {
      0.5, 0.25, 0.75, // pixel 1
      1.0, 0.0, 0.0    // pixel 2
  };
  unsigned char output[6];

  exposure_tm.apply(framebuffer, output, 2, 3, 3);

  for (int i = 0; i < 2; ++i) {
      ColorRGB original(framebuffer[i * 3], framebuffer[i * 3 + 1], framebuffer[i * 3 + 2]);
      ColorRGB expected = exposure_tm.convertToLDR(original);

      EXPECT_EQ(output[i * 3], static_cast<unsigned char>(expected.r * 255.999));
      EXPECT_EQ(output[i * 3 + 1], static_cast<unsigned char>(expected.g * 255.999));
      EXPECT_EQ(output[i * 3 + 2], static_cast<unsigned char>(expected.b * 255.999));
  }
}

TEST(ToneMappingTest, ApplyRGBtoRGBA) {
  ExposureToneMapping exposure_tm(1.0);

  double framebuffer[] = {
      0.5, 0.25, 0.75, // pixel 1
      1.0, 0.0, 0.0    // pixel 2
  };
  unsigned char output[8];

  exposure_tm.apply(framebuffer, output, 2, 3, 4);

  for (int i = 0; i < 2; ++i) {
      ColorRGB original(framebuffer[i * 3], framebuffer[i * 3 + 1], framebuffer[i * 3 + 2]);
      ColorRGB expected = exposure_tm.convertToLDR(original);
      EXPECT_EQ(output[i * 4], static_cast<unsigned char>(expected.r * 255.999));
      EXPECT_EQ(output[i * 4 + 1], static_cast<unsigned char>(expected.g * 255.999));
      EXPECT_EQ(output[i * 4 + 2], static_cast<unsigned char>(expected.b * 255.999));
      EXPECT_EQ(output[i * 4 + 3], 255);
  }
}

TEST(ToneMappingTest, ApplyRGBAToGrayscale) {
  ExposureToneMapping exposure_tm(1.0);

  double framebuffer[] = {
      0.5, 0.25, 0.75, 1.0, // pixel 1
      1.0, 0.0, 0.0, 0.5    // pixel 2
  };
  unsigned char output[2];

  exposure_tm.apply(framebuffer, output, 2, 4, 1);

  for (int i = 0; i < 2; ++i) {
      ColorRGB original(framebuffer[i * 4], framebuffer[i * 4 + 1], framebuffer[i * 4 + 2]);
      unsigned char expected_gray = static_cast<unsigned char>(exposure_tm.convertToLDR(original).grayscale() * 255.999);
      EXPECT_EQ(output[i], expected_gray);
  }
}

TEST(ToneMappingTest, ApplyRGBAtoRGB) {
  ExposureToneMapping exposure_tm(1.0);

    double framebuffer[] = {
        0.5, 0.25, 0.75, 1.0, // pixel 1
        1.0, 0.0, 0.0, 0.5    // pixel 2
    };
    unsigned char output[6];

    exposure_tm.apply(framebuffer, output, 2, 4, 3);

    for (int i = 0; i < 2; ++i) {
        ColorRGB original(framebuffer[i * 4], framebuffer[i * 4 + 1], framebuffer[i * 4 + 2]);
        ColorRGB expected = exposure_tm.convertToLDR(original);

        EXPECT_EQ(output[i * 3], static_cast<unsigned char>(expected.r * 255.999));
        EXPECT_EQ(output[i * 3 + 1], static_cast<unsigned char>(expected.g * 255.999));
        EXPECT_EQ(output[i * 3 + 2], static_cast<unsigned char>(expected.b * 255.999));
    }
}

TEST(ToneMappingTest, ApplyRGBAToRGBA) {
  ExposureToneMapping exposure_tm(1.0);

  double framebuffer[] = {
      0.5, 0.25, 0.75, 1.0, // pixel 1
      1.0, 0.0, 0.0, 0.5    // pixel 2
  };

  unsigned char output[8];

  exposure_tm.apply(framebuffer, output, 2, 4, 4);

  for (int i = 0; i < 2; ++i) {
      ColorRGB original(framebuffer[i * 4], framebuffer[i * 4 + 1], framebuffer[i * 4 + 2]);
      ColorRGB expected = exposure_tm.convertToLDR(original);
      EXPECT_EQ(output[i * 4], static_cast<unsigned char>(expected.r * 255.999));
      EXPECT_EQ(output[i * 4 + 1], static_cast<unsigned char>(expected.g * 255.999));
      EXPECT_EQ(output[i * 4 + 2], static_cast<unsigned char>(expected.b * 255.999));
      EXPECT_EQ(output[i * 4 + 3], static_cast<unsigned char>(framebuffer[i * 4 + 3] * 255.999));
  }
}

TEST(ToneMappingTest, InvalidChannelCountThrows) {
  ExposureToneMapping exposure_tm(1.0);

  double framebuffer[] = {0.5, 0.25, 0.75};
  unsigned char output[3];

  EXPECT_THROW(
      exposure_tm.apply(framebuffer, output, 1, 3, 2), std::runtime_error);
}

TEST(ToneMappingTest, StringToToneMappingMapContainsExpectedValues) {
  const auto& map = stringToToneMappingMap();
  EXPECT_EQ(map.at("Luminance Reinhard"), ToneMapping::LUMINANCE_REINHARD);
  EXPECT_EQ(map.at("White Point Reinhard"), ToneMapping::WHITE_POINT_REINHARD);
  EXPECT_EQ(map.at("Exposure"), ToneMapping::EXPOSURE);
  EXPECT_EQ(map.at("Uncharted2"), ToneMapping::UNCHARTED2);
  EXPECT_EQ(map.at("ACES"), ToneMapping::ACES);
  EXPECT_EQ(map.at("None"), ToneMapping::NONE);
}

TEST(ToneMappingTest, ToneMappingToStringReturnsCorrectNames) {
  EXPECT_EQ(toneMappingToString(ToneMapping::LUMINANCE_REINHARD), "Luminance Reinhard");
  EXPECT_EQ(toneMappingToString(ToneMapping::WHITE_POINT_REINHARD), "White Point Reinhard");
  EXPECT_EQ(toneMappingToString(ToneMapping::EXPOSURE), "Exposure");
  EXPECT_EQ(toneMappingToString(ToneMapping::UNCHARTED2), "Uncharted2");
  EXPECT_EQ(toneMappingToString(ToneMapping::ACES), "ACES");
  EXPECT_EQ(toneMappingToString(ToneMapping::NONE), "None");
  EXPECT_EQ(toneMappingToString(static_cast<ToneMapping>(999)), "None"); // Invalid mapping
}

TEST(ToneMappingTest, StringToToneMappingReturnsCorrectEnums) {
  EXPECT_EQ(stringToToneMapping("Luminance Reinhard"), ToneMapping::LUMINANCE_REINHARD);
  EXPECT_EQ(stringToToneMapping("White Point Reinhard"), ToneMapping::WHITE_POINT_REINHARD);
  EXPECT_EQ(stringToToneMapping("Exposure"), ToneMapping::EXPOSURE);
  EXPECT_EQ(stringToToneMapping("Uncharted2"), ToneMapping::UNCHARTED2);
  EXPECT_EQ(stringToToneMapping("ACES"), ToneMapping::ACES);
  EXPECT_EQ(stringToToneMapping("None"), ToneMapping::NONE);
  EXPECT_EQ(stringToToneMapping("InvalidName"), ToneMapping::NONE);
}

TEST(ToneMappingTest, AvailableToneMappingNamesContainsAll) {
  auto names = availableToneMappingNames();
  EXPECT_EQ(names.size(), stringToToneMappingMap().size());
  for (const auto& [key, _] : stringToToneMappingMap()) {
      EXPECT_NE(std::find(names.begin(), names.end(), key), names.end());
  }
}
