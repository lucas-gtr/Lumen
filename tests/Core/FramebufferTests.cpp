#include <gtest/gtest.h>
#include <Eigen/Core>
#include "Core/Framebuffer.hpp"
#include "Core/ColorUtils.hpp"

TEST(FramebufferTest, ConstructorAllocatesMemory) {
    Framebuffer fb({10, 10, 3});
    const double* data = fb.getFramebuffer();
    EXPECT_NE(data, nullptr);
}

TEST(FramebufferTest, SetFramebufferPropertiesResizesCorrectly) {
    Framebuffer fb({10, 10, 3});
    const double* before = fb.getFramebuffer();
    fb.setFramebufferProperties({20, 20, 3});
    const double* after = fb.getFramebuffer();
    EXPECT_NE(before, after);

    fb.setFramebufferProperties({20, 20, 4});
    after = fb.getFramebuffer();
    EXPECT_NE(before, after);

    fb.setFramebufferProperties({20, 10, 4});
    after = fb.getFramebuffer();
    EXPECT_NE(before, after);
}

TEST(FramebufferTest, SetPixelColorAffectsFramebuffer3Channels) {
    Framebuffer fb({2, 2, 3});
    Eigen::Vector4d color(0.5, 0.25, 0.75, 1.0);
    fb.setPixelColor({1, 1}, color, 1.0);
    const double* image = fb.getFramebuffer();
    int index = (1 * 2 + 1) * 3;
    EXPECT_EQ(image[index + 0], 0.5);
    EXPECT_EQ(image[index + 1], 0.25);
    EXPECT_EQ(image[index + 2], 0.75);
}

TEST(FramebufferTest, SetPixelColorAffectsFramebuffer1Channel) {
    Framebuffer fb({2, 2, 1});
    Eigen::Vector4d color(0.5, 0.25, 0.75, 1.0);
    fb.setPixelColor({1, 1}, color, 1.0);
    const double* image = fb.getFramebuffer();
    int index = (1 * 2 + 1) * 1;
    EXPECT_EQ(image[index], toGrayscale(color));
}

TEST(FramebufferTest, SetPixelColorAffectsFramebuffer2Channels) {
    Framebuffer fb({2, 2, 2});
    Eigen::Vector4d color(0.5, 0.25, 0.75, 1.0);
    fb.setPixelColor({1, 1}, color, 1.0);
    const double* image = fb.getFramebuffer();
    int index = (1 * 2 + 1) * 2;

    for (int i = 0; i < 2 * 2 * 2; ++i) {
        EXPECT_EQ(image[i], 0);
    }
}

TEST(FramebufferTest, SetPixelColorAffectsFramebuffer4Channels) {
    Framebuffer fb({2, 2, 4});
    Eigen::Vector4d color(0.5, 0.25, 0.75, 1.0);
    fb.setPixelColor({1, 1}, color, 1.0);
    const double* image = fb.getFramebuffer();
    int index = (1 * 2 + 1) * 4;
    EXPECT_EQ(image[index + 0], 0.5);
    EXPECT_EQ(image[index + 1], 0.25);
    EXPECT_EQ(image[index + 2], 0.75);
    EXPECT_EQ(image[index + 3], 1.0);
}

TEST(FramebufferTest, SetPixelColorOutOfBoundsDoesNothing) {
    Framebuffer fb({2, 2, 3});
    Eigen::Vector4d color(1.0, 1.0, 1.0, 1.0);
    fb.setPixelColor({-1, 0}, color, 1.0);
    fb.setPixelColor({2, 0}, color, 1.0); 
    fb.setPixelColor({0, -1}, color, 1.0);
    fb.setPixelColor({0, 2}, color, 1.0);
    const double* image = fb.getFramebuffer();
    for (int i = 0; i < 2 * 2 * 3; ++i) {
        EXPECT_EQ(image[i], 0);
    }
}

TEST(FramebufferTest, DestructorFreesMemory) {
    Framebuffer* fb = new Framebuffer({10, 10, 3});
    const double* data = fb->getFramebuffer();
    EXPECT_NE(data, nullptr);
    delete fb;
}
