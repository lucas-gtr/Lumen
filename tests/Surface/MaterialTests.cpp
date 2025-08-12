#include <gtest/gtest.h>
#include <memory>

#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

TEST(MaterialTest, DefaultConstructor) {
    Material material;
    EXPECT_EQ(material.getDiffuse({0, 0}), ColorRGB(1.0, 1.0, 1.0));
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.5, 0.5, 1.0));
}

TEST(MaterialTest, SetDiffuseTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.5, 0.5, 0.5));
    material.setDiffuseTexture(&texture);
    EXPECT_EQ(material.getDiffuse({0, 0}), ColorRGB(0.5, 0.5, 0.5));

    material.setDiffuseTexture(nullptr);
    EXPECT_EQ(material.getDiffuse({0, 0}), ColorRGB(1.0, 1.0, 1.0)); // Default texture
}

TEST(MaterialTest, SetNormalTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.0, 1.0, 0.0));
    material.setNormalTexture(&texture);
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.0, 1.0, 0.0));

    material.setNormalTexture(nullptr);
    EXPECT_EQ(material.getNormal({0, 0}), ColorRGB(0.5, 0.5, 1.0)); // Default normal texture
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
    EXPECT_EQ(defaultTexture->getValue3d({0, 0}), ColorRGB(1.0, 1.0, 1.0));
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

TEST(MaterialTest, DefaultRoughness) {
    Material material;
    EXPECT_DOUBLE_EQ(material.getRoughness({0, 0}), 0.5);
    EXPECT_FALSE(material.isUsingTextureRoughness());
}

TEST(MaterialTest, SetRoughnessValue) {
    Material material;
    material.setRoughnessValue(0.8);
    EXPECT_DOUBLE_EQ(material.getRoughness({0, 0}), 0.8);
    EXPECT_FALSE(material.isUsingTextureRoughness());
}

TEST(MaterialTest, SetRoughnessTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.2, 0.2, 0.2));
    material.setRoughnessTexture(&texture);
    material.setUseTextureRoughness(true);
    EXPECT_TRUE(material.isUsingTextureRoughness());
    EXPECT_DOUBLE_EQ(material.getRoughness({0, 0}), 0.2);

    EXPECT_EQ(material.getRoughnessTexture(), &texture);

    material.setRoughnessTexture(nullptr);
    EXPECT_DOUBLE_EQ(material.getRoughness({0, 0}), 0.5); // Default roughness value
    EXPECT_TRUE(material.isUsingTextureRoughness());
}

TEST(MaterialTest, DefaultMetallic) {
    Material material;
    EXPECT_DOUBLE_EQ(material.getMetalness({0, 0}), 0.0);
    EXPECT_FALSE(material.isUsingTextureMetallic());
}

TEST(MaterialTest, SetMetallicValue) {
    Material material;
    material.setMetallicValue(0.9);
    EXPECT_DOUBLE_EQ(material.getMetalness({0, 0}), 0.9);
    EXPECT_FALSE(material.isUsingTextureMetallic());
}

TEST(MaterialTest, SetMetallicTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.4, 0.4, 0.4));
    material.setMetallicTexture(&texture);
    material.setUseTextureMetallic(true);
    EXPECT_TRUE(material.isUsingTextureMetallic());
    EXPECT_DOUBLE_EQ(material.getMetalness({0, 0}), 0.4);

    EXPECT_EQ(material.getMetallicTexture(), &texture);

    material.setMetallicTexture(nullptr);
    EXPECT_DOUBLE_EQ(material.getMetalness({0, 0}), 0.0); // Default metallic value
    EXPECT_TRUE(material.isUsingTextureMetallic());
}

TEST(MaterialTest, DefaultEmissive) {
    Material material;
    EXPECT_EQ(material.getEmissive({0, 0}), ColorRGB(0.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(material.getEmissiveIntensity(), 0.0);
}

TEST(MaterialTest, SetEmissiveTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(1.0, 0.5, 0.0));
    material.setEmissiveTexture(&texture);
    EXPECT_EQ(material.getEmissive({0, 0}), ColorRGB(1.0, 0.5, 0.0));

    EXPECT_EQ(material.getEmissiveTexture(), &texture);

    material.setEmissiveTexture(nullptr);
    EXPECT_EQ(material.getEmissive({0, 0}), ColorRGB(0.0, 0.0, 0.0)); // Default emissive texture
}

TEST(MaterialTest, SetEmissiveIntensity) {
    Material material;
    material.setEmissiveIntensity(2.5);
    EXPECT_DOUBLE_EQ(material.getEmissiveIntensity(), 2.5);
}

TEST(MaterialTest, DefaultTransmission) {
    Material material;
    EXPECT_DOUBLE_EQ(material.getTransmission({0, 0}), 0.0);
    EXPECT_FALSE(material.isUsingTextureTransmission());
}

TEST(MaterialTest, SetTransmissionValue) {
    Material material;
    material.setTransmissionValue(0.6);
    EXPECT_DOUBLE_EQ(material.getTransmission({0, 0}), 0.6);
    EXPECT_FALSE(material.isUsingTextureTransmission());
}

TEST(MaterialTest, SetTransmissionTexture) {
    Material material;
    Texture texture;
    texture.setValue(ColorRGB(0.7, 0.7, 0.7));
    material.setTransmissionTexture(&texture);
    material.setUseTextureTransmission(true);
    EXPECT_TRUE(material.isUsingTextureTransmission());
    EXPECT_DOUBLE_EQ(material.getTransmission({0, 0}), 0.7);

    EXPECT_EQ(material.getTransmissionTexture(), &texture);
  
    material.setTransmissionTexture(nullptr);
    EXPECT_DOUBLE_EQ(material.getTransmission({0, 0}), 0.0); // Default transmission value
    EXPECT_TRUE(material.isUsingTextureTransmission());
  }

TEST(MaterialTest, DefaultIOR) {
    Material material;
    EXPECT_DOUBLE_EQ(material.getIndexOfRefraction(), 1.0);
}

TEST(MaterialTest, SetIOR) {
    Material material;
    material.setIndexOfRefraction(1.5);
    EXPECT_DOUBLE_EQ(material.getIndexOfRefraction(), 1.5);
}

TEST(MaterialTest, ObserverNotifiedOnChange) {
    Material material;
    bool notified = false;

    material.getMaterialChangedObserver().add([&](const Material* m) {
        notified = true;
        EXPECT_EQ(m, &material);
    });

    Texture texture;
    texture.setValue(ColorRGB(0.3, 0.3, 0.3));
    material.setDiffuseTexture(&texture);

    EXPECT_TRUE(notified);
}

TEST(MaterialTest, ObserverMultipleCallbacks) {
    Material material;
    int counter = 0;

    material.getMaterialChangedObserver().add([&](const Material*) { counter += 1; });
    material.getMaterialChangedObserver().add([&](const Material*) { counter += 10; });

    material.setRoughnessValue(0.9);

    EXPECT_EQ(counter, 11);
}
