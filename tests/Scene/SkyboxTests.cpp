#include <gtest/gtest.h>
#include <memory>

#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"
#include "Core/ColorUtils.hpp"

TEST(SkyboxTest, DefaultConstructorUsesDefaultColor) {
  Skybox skybox;

  lin::Vec3d direction(1.0, 0.0, 0.0);
  ColorRGBA color = skybox.getColor(direction);

  double r = 0.65;
  convertToLinearSpace(r);
  double g = 0.65;
  convertToLinearSpace(g);
  double b = 0.9;
  convertToLinearSpace(b);

  EXPECT_DOUBLE_EQ(color.r, r);
  EXPECT_DOUBLE_EQ(color.g, g);
  EXPECT_DOUBLE_EQ(color.b, b);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(SkyboxTest, SetTextureWithSingleColorReturnsCorrectColor) {
  Skybox skybox;

  lin::Vec3d direction(0.0, 1.0, 0.0);
  ColorRGB color_rgb(0.2, 0.4, 0.6);
  auto texture = std::make_shared<Texture>(color_rgb);
  skybox.setTexture(texture);

  ColorRGBA color = skybox.getColor(direction);

  convertToLinearSpace(color_rgb.r);
  convertToLinearSpace(color_rgb.g);
  convertToLinearSpace(color_rgb.b);

  EXPECT_DOUBLE_EQ(color.r, color_rgb.r);
  EXPECT_DOUBLE_EQ(color.g, color_rgb.g);
  EXPECT_DOUBLE_EQ(color.b, color_rgb.b);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(SkyboxTest, GetUVCoordinatesIsCorrectlyComputed) {
  Skybox skybox;
  TextureUV uv;

  lin::Vec3d direction1(0.0, 0.0, -1.0);
  uv = skybox.getUVCoordinates(direction1);

  EXPECT_NEAR(uv.u, 0.25, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  lin::Vec3d direction2(0.0, 0.0, 1.0);
  uv = skybox.getUVCoordinates(direction2);
  
  EXPECT_NEAR(uv.u, 0.75, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  lin::Vec3d direction3(1.0, 0.0, 0.0);
  uv = skybox.getUVCoordinates(direction3);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  lin::Vec3d direction4(-1.0, 0.0, 0.0);
  uv = skybox.getUVCoordinates(direction4);
  EXPECT_NEAR(uv.u, 1.0, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  lin::Vec3d direction5(0.0, 1.0, 0.0);
  uv = skybox.getUVCoordinates(direction5);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 0.0, 1e-6);

  lin::Vec3d direction6(0.0, -1.0, 0.0);
  uv = skybox.getUVCoordinates(direction6);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 1.0, 1e-6);

}

TEST(SkyboxTest, ReturnsDefaultColorAfterTextureIsResetToNull) {
  Skybox skybox;

  auto texture = std::make_shared<Texture>(ColorRGBA(0.1, 0.2, 0.3, 1.0));
  skybox.setTexture(texture);

  skybox.setTexture(nullptr);

  double r = 0.1;
  convertToLinearSpace(r);
  double g = 0.2;
  convertToLinearSpace(g);
  double b = 0.3;
  convertToLinearSpace(b);

  lin::Vec3d direction(0.0, 0.0, 1.0);
  ColorRGBA color = skybox.getColor(direction);

  EXPECT_DOUBLE_EQ(color.r, r);
  EXPECT_DOUBLE_EQ(color.g, g);
  EXPECT_DOUBLE_EQ(color.b, b);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(SkyboxTest, GetTexture) {
  Skybox skybox;

  auto texture = std::make_shared<Texture>(ColorRGB(0.5, 0.5, 0.5));

  EXPECT_NE(skybox.getTexture(), nullptr);
  skybox.setTexture(texture);

  const Texture* retrieved_texture = skybox.getTexture();
  EXPECT_NE(retrieved_texture, nullptr);
}
