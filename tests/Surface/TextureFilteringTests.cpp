#include "Surface/TextureFiltering.hpp"

#include <gtest/gtest.h>

TEST(TextureFilteringTest, NearestFilter) {
  TextureUV uv_coord{0.5, 0.49};
  Resolution resolution{4, 4};
  PixelCoord pixel_coord = TextureSampling::sampleNearest(uv_coord, resolution);
  EXPECT_EQ(pixel_coord.x, 2);
  EXPECT_EQ(pixel_coord.y, 1);
}

TEST(TextureFilteringTest, BilinearFilter) {
  TextureUV uv_coord{0.25, 0.75};
  Resolution resolution{4, 4};
  TextureSampling::BilinearSampleInfo sample_info = TextureSampling::sampleBilinear(uv_coord, resolution);
  EXPECT_EQ(sample_info.x0, 0);
  EXPECT_EQ(sample_info.x1, 1);
  EXPECT_EQ(sample_info.y0, 2);
  EXPECT_EQ(sample_info.y1, 3);
  EXPECT_NEAR(sample_info.dx, 0.5, 1e-6);
  EXPECT_NEAR(sample_info.dy, 0.5, 1e-6);

  uv_coord = {0.125, 0.625};
  sample_info = TextureSampling::sampleBilinear(uv_coord, resolution);
  EXPECT_EQ(sample_info.x0, 0);
  EXPECT_EQ(sample_info.x1, 1);
  EXPECT_EQ(sample_info.y0, 2);
  EXPECT_EQ(sample_info.y1, 3);
  EXPECT_NEAR(sample_info.dx, 0.0, 1e-6);
  EXPECT_NEAR(sample_info.dy, 0.0, 1e-6);

  uv_coord = {0.0, 1.0};
  sample_info = TextureSampling::sampleBilinear(uv_coord, resolution);
  EXPECT_EQ(sample_info.x0, 0);
  EXPECT_EQ(sample_info.x1, 0);
  EXPECT_EQ(sample_info.y0, 3);
  EXPECT_EQ(sample_info.y1, 3);
  EXPECT_NEAR(sample_info.dx, 0.0, 1e-6);
  EXPECT_NEAR(sample_info.dy, 0.5, 1e-6);
}