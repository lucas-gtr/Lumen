#include "Surface/TextureWrapping.hpp"

#include <gtest/gtest.h>

TEST(TextureWrappingTest, WrapRepeat) {
  TextureUV uv_coord{1.4, -0.6};
  TextureSampling::wrapCoordinates(uv_coord, TextureSampling::TextureWrapping::REPEAT);
  EXPECT_NEAR(uv_coord.u, 0.4, 1e-6);
  EXPECT_NEAR(uv_coord.v, 0.4, 1e-6);
}

TEST(TextureWrappingTest, WrapMirroredRepeat) {
  TextureUV uv_coord{1.1, -0.1};
  TextureSampling::wrapCoordinates(uv_coord, TextureSampling::TextureWrapping::MIRRORED_REPEAT);
  EXPECT_NEAR(uv_coord.u, 0.9, 1e-6);
  EXPECT_NEAR(uv_coord.v, 0.1, 1e-6);
}

TEST(TextureWrappingTest, WrapClampToEdge) {
  TextureUV uv_coord{1.5, -0.5};
  TextureSampling::wrapCoordinates(uv_coord, TextureSampling::TextureWrapping::CLAMP_TO_EDGE);
  EXPECT_NEAR(uv_coord.u, 1.0, 1e-6);
  EXPECT_NEAR(uv_coord.v, 0.0, 1e-6);
}

TEST(TextureWrappingTest, WrapClampToBorder) {
  TextureUV uv_coord{1.5, -0.5};
  TextureSampling::wrapCoordinates(uv_coord, TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  EXPECT_EQ(uv_coord.u, -1.0);
  EXPECT_EQ(uv_coord.v, -1.0);
}

TEST(TextureWrappingTest, WrapClampToBorderWithValidCoords) {
  TextureUV uv_coord{0.5, 0.5};
  TextureSampling::wrapCoordinates(uv_coord, TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  EXPECT_NEAR(uv_coord.u, 0.5, 1e-6);
  EXPECT_NEAR(uv_coord.v, 0.5, 1e-6);
}