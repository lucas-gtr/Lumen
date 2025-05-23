#include <gtest/gtest.h>
#include <memory>

#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"
#include "Core/ColorUtils.hpp"

TEST(SkyboxTest, DefaultConstructorUsesDefaultColor) {
  Skybox skybox;

  Eigen::Vector3d direction(1.0, 0.0, 0.0);
  Eigen::Vector4d color = skybox.getColor(direction);

  double r = 0.65;
  convertToLinearSpace(r);
  double g = 0.65;
  convertToLinearSpace(g);
  double b = 0.9;
  convertToLinearSpace(b);

  EXPECT_DOUBLE_EQ(color.x(), r);
  EXPECT_DOUBLE_EQ(color.y(), g);
  EXPECT_DOUBLE_EQ(color.z(), b);
  EXPECT_DOUBLE_EQ(color.w(), 1.0);
}

TEST(SkyboxTest, SetTextureWithSingleColorReturnsCorrectColor) {
  Skybox skybox;

  Eigen::Vector3d direction(0.0, 1.0, 0.0);
  Eigen::Vector3d color_rgb(0.2, 0.4, 0.6);
  auto texture = std::make_shared<Texture>(color_rgb);
  skybox.setTexture(texture);

  Eigen::Vector4d color = skybox.getColor(direction);

  convertToLinearSpace(color_rgb.x());
  convertToLinearSpace(color_rgb.y());
  convertToLinearSpace(color_rgb.z());

  EXPECT_DOUBLE_EQ(color.x(), color_rgb.x());
  EXPECT_DOUBLE_EQ(color.y(), color_rgb.y());
  EXPECT_DOUBLE_EQ(color.z(), color_rgb.z());
  EXPECT_DOUBLE_EQ(color.w(), 1.0);
}

TEST(SkyboxTest, GetUVCoordinatesIsCorrectlyComputed) {
  Skybox skybox;
  TextureUV uv;

  Eigen::Vector3d direction1(0.0, 0.0, -1.0);
  uv = skybox.getUVCoordinates(direction1);

  EXPECT_NEAR(uv.u, 0.25, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  Eigen::Vector3d direction2(0.0, 0.0, 1.0);
  uv = skybox.getUVCoordinates(direction2);
  
  EXPECT_NEAR(uv.u, 0.75, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  Eigen::Vector3d direction3(1.0, 0.0, 0.0);
  uv = skybox.getUVCoordinates(direction3);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  Eigen::Vector3d direction4(-1.0, 0.0, 0.0);
  uv = skybox.getUVCoordinates(direction4);
  EXPECT_NEAR(uv.u, 1.0, 1e-6);
  EXPECT_NEAR(uv.v, 0.5, 1e-6);

  Eigen::Vector3d direction5(0.0, 1.0, 0.0);
  uv = skybox.getUVCoordinates(direction5);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 0.0, 1e-6);

  Eigen::Vector3d direction6(0.0, -1.0, 0.0);
  uv = skybox.getUVCoordinates(direction6);
  EXPECT_NEAR(uv.u, 0.5, 1e-6);
  EXPECT_NEAR(uv.v, 1.0, 1e-6);

}

TEST(SkyboxTest, ReturnsDefaultColorAfterTextureIsResetToNull) {
  Skybox skybox;

  auto texture = std::make_shared<Texture>(Eigen::Vector4d(0.1, 0.2, 0.3, 1.0));
  skybox.setTexture(texture);

  skybox.setTexture(nullptr);

  double r = 0.1;
  convertToLinearSpace(r);
  double g = 0.2;
  convertToLinearSpace(g);
  double b = 0.3;
  convertToLinearSpace(b);

  Eigen::Vector3d direction(0.0, 0.0, 1.0);
  Eigen::Vector4d color = skybox.getColor(direction);

  EXPECT_DOUBLE_EQ(color.x(), r);
  EXPECT_DOUBLE_EQ(color.y(), g);
  EXPECT_DOUBLE_EQ(color.z(), b);
  EXPECT_DOUBLE_EQ(color.w(), 1.0);
}
