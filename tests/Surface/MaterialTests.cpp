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