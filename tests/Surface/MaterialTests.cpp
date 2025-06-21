#include <gtest/gtest.h>
#include <memory>
#include "Surface/Material.hpp"

TEST(MaterialTest, DefaultConstructor) {
    Material material;
    EXPECT_EQ(material.getDiffuse({0, 0}), ColorRGBA(1.0, 1.0, 1.0, 1.0));
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.5, 0.5, 1.0));
}

TEST(MaterialTest, SetDiffuseTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.5, 0.5, 0.5));
    material.setDiffuseTexture(&texture);
    EXPECT_EQ(material.getDiffuse({0, 0}), ColorRGBA(0.5, 0.5, 0.5, 1.0));
}

TEST(MaterialTest, SetNormalTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.0, 1.0, 0.0));
    material.setNormalTexture(&texture);
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.0, 1.0, 0.0));
}

TEST(MaterialTest, GetDiffuseTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.3, 0.6, 0.9));
    material.setDiffuseTexture(&texture);
    EXPECT_EQ(material.getDiffuseTexture(), &texture);
}

TEST(MaterialTest, GetDiffuseTextureDefault) {
    Material material;
    const Texture* defaultTexture = material.getDiffuseTexture();
    EXPECT_EQ(defaultTexture->getValue4d({0, 0}), ColorRGBA(1.0, 1.0, 1.0, 1.0));
}

TEST(MaterialTest, GetNormalTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(1.0, 0.0, 0.0));
    material.setNormalTexture(&texture);
    EXPECT_EQ(material.getNormalTexture(), &texture);
}

TEST(MaterialTest, GetNormalTextureDefault) {
    Material material;
    const Texture* defaultTexture = material.getNormalTexture();
    EXPECT_EQ(defaultTexture->getValue3d({0, 0}), ColorRGB(0.5, 0.5, 1.0));
}