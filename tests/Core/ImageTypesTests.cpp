#include <gtest/gtest.h>
#include "Core/ImageTypes.hpp"

TEST(ImagePropertiesTest, BufferSize) {
  ImageProperties img{2, 3, 4};
  EXPECT_EQ(img.bufferSize(), 24);
}

TEST(PixelCoordTest, DefaultValues) {
  PixelCoord coord;
  EXPECT_EQ(coord.x, 0);
  EXPECT_EQ(coord.y, 0);
}

TEST(ResolutionTest, DefaultValues) {
  Resolution res;
  EXPECT_EQ(res.width, 1);
  EXPECT_EQ(res.height, 1);
}

TEST(TextureUVTest, DefaultValues) {
  TextureUV uv;
  EXPECT_DOUBLE_EQ(uv.u, 0.0);
  EXPECT_DOUBLE_EQ(uv.v, 0.0);
}


