#include <gtest/gtest.h>
#include <memory>
#include "Surface/Material.hpp"

TEST(MaterialTest, DefaultConstructor) {
    Material material;
    EXPECT_EQ(material.getAlbedo({0, 0}), ColorRGBA(1.0, 1.0, 1.0, 1.0));
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.5, 0.5, 1.0));
}

TEST(MaterialTest, SetAlbedoTexture) {
    Material material;
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(ColorRGB(0.5, 0.5, 0.5));
    material.setAlbedoTexture(texture);
    EXPECT_EQ(material.getAlbedo({0, 0}), ColorRGBA(0.5, 0.5, 0.5, 1.0));
}

TEST(MaterialTest, SetNormalTexture) {
    Material material;
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(ColorRGB(0.0, 1.0, 0.0));
    material.setNormalTexture(texture);
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.0, 1.0, 0.0));
}

TEST(MaterialTest, GetAlbedoTexture) {
    Material material;
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(ColorRGB(0.3, 0.6, 0.9));
    material.setAlbedoTexture(texture);
    EXPECT_EQ(&material.getAlbedoTexture(), texture.get());
}

TEST(MaterialTest, GetAlbedoTextureDefault) {
    Material material;
    const Texture& defaultTexture = material.getAlbedoTexture();
    EXPECT_EQ(defaultTexture.getValue4d({0, 0}), ColorRGBA(1.0, 1.0, 1.0, 1.0));
}

TEST(MaterialTest, GetNormalTexture) {
    Material material;
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(ColorRGB(1.0, 0.0, 0.0));
    material.setNormalTexture(texture);
    EXPECT_EQ(&material.getNormalTexture(), texture.get());
}

TEST(MaterialTest, GetNormalTextureDefault) {
    Material material;
    const Texture& defaultTexture = material.getNormalTexture();
    EXPECT_EQ(defaultTexture.getValue3d({0, 0}), ColorRGB(0.5, 0.5, 1.0));
}