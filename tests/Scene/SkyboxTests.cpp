#include <gtest/gtest.h>
#include <memory>

#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"
#include "Core/ColorUtils.hpp"

TEST(SkyboxTest, DefaultConstructorUsesDefaultColor) {
  Skybox skybox;

  linalg::Vec3d direction(1.0, 0.0, 0.0);
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

  linalg::Vec3d direction(0.0, 1.0, 0.0);
  ColorRGB color_rgb(0.2, 0.4, 0.6);
  Texture texture;
  texture.setValue(color_rgb);
  texture.setColorSpace(ColorSpace::LINEAR);
  skybox.setTexture(&texture);

  ColorRGBA color = skybox.getColor(direction);

  convertToLinearSpace(color_rgb);

  EXPECT_DOUBLE_EQ(color.r, color_rgb.r);
  EXPECT_DOUBLE_EQ(color.g, color_rgb.g);
  EXPECT_DOUBLE_EQ(color.b, color_rgb.b);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(SkyboxTest, GetUVCoordinatesIsCorrectlyComputed) {
  Skybox skybox;
  TextureUV uv;

  linalg::Vec3d direction1(0.0, 0.0, -1.0);
  uv = skybox.GetUvCoordinates(direction1);

  EXPECT_NEAR(uv.u, 0.25, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  linalg::Vec3d direction2(0.0, 0.0, 1.0);
  uv = skybox.GetUvCoordinates(direction2);
  
  EXPECT_NEAR(uv.u, 0.75, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  linalg::Vec3d direction3(1.0, 0.0, 0.0);
  uv = skybox.GetUvCoordinates(direction3);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  linalg::Vec3d direction4(-1.0, 0.0, 0.0);
  uv = skybox.GetUvCoordinates(direction4);
  EXPECT_NEAR(uv.u, 1.0, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  linalg::Vec3d direction5(0.0, 1.0, 0.0);
  uv = skybox.GetUvCoordinates(direction5);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 0.0, 1e-6);

  linalg::Vec3d direction6(0.0, -1.0, 0.0);
  uv = skybox.GetUvCoordinates(direction6);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 1.0, 1e-6);

}

TEST(SkyboxTest, ReturnsDefaultColorAfterTextureIsResetToNull) {
  Skybox skybox;

  Texture texture;
  texture.setValue(ColorRGB(0.1, 0.2, 0.3));
  texture.setColorSpace(ColorSpace::LINEAR);
  skybox.setTexture(&texture);

  skybox.setTexture(nullptr);

  double r = 0.1;
  convertToLinearSpace(r);
  double g = 0.2;
  convertToLinearSpace(g);
  double b = 0.3;
  convertToLinearSpace(b);

  linalg::Vec3d direction(0.0, 0.0, 1.0);
  ColorRGBA color = skybox.getColor(direction);

  EXPECT_DOUBLE_EQ(color.r, r);
  EXPECT_DOUBLE_EQ(color.g, g);
  EXPECT_DOUBLE_EQ(color.b, b);
  EXPECT_DOUBLE_EQ(color.a, 1.0);
}

TEST(SkyboxTest, GetTexture) {
  Skybox skybox;

  Texture texture;

  EXPECT_NE(skybox.getTexture(), nullptr);
  skybox.setTexture(&texture);

  const Texture* retrieved_texture = skybox.getTexture();
  EXPECT_NE(retrieved_texture, nullptr);
}

TEST(SkyboxTest, TextureObserverNotifiesOnTextureChange) {
  Skybox skybox;
  Texture texture;

  bool notified = false;
  skybox.getTextureObserver().add([&]() { notified = true; });

  skybox.setTexture(&texture);
  EXPECT_TRUE(notified);

  notified = false;
  skybox.setTexture(nullptr);
  EXPECT_FALSE(notified); // No notification expected when setting to nullptr
}
