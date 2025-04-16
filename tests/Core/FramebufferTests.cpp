#include <gtest/gtest.h>
#include <Eigen/Core>
#include "Core/Framebuffer.hpp"

TEST(FramebufferTest, ConstructorAllocatesMemory) {
    Framebuffer fb({10, 10, 3});
    fb.generateImage();
    const unsigned char* data = fb.getImage();
    EXPECT_NE(data, nullptr);
}

TEST(FramebufferTest, SetFramebufferPropertiesResizesCorrectly) {
    Framebuffer fb({10, 10, 3});
    fb.generateImage();
    const unsigned char* before = fb.getImage();
    fb.setFramebufferProperties({20, 20, 3});
    fb.generateImage();
    const unsigned char* after = fb.getImage();
    EXPECT_NE(before, after);

    fb.setFramebufferProperties({20, 20, 4});
    fb.generateImage();
    after = fb.getImage();
    EXPECT_NE(before, after);

    fb.setFramebufferProperties({20, 10, 4});
    fb.generateImage();
    after = fb.getImage();
    EXPECT_NE(before, after);
}

TEST(FramebufferTest, SetPixelColorAffectsFramebuffer) {
    Framebuffer fb({2, 2, 3});
    Eigen::Vector3d color(0.5, 0.25, 0.75);
    fb.setPixelColor(1, 1, color, 1.0);
    fb.generateImage();
    const unsigned char* image = fb.getImage();
    int index = (1 * 2 + 1) * 3;
    std::cout << 0.5 * 255.0 + 0.5 << std::endl;
    EXPECT_EQ(image[index + 0], static_cast<unsigned char>(127));
    EXPECT_EQ(image[index + 1], static_cast<unsigned char>(63));
    EXPECT_EQ(image[index + 2], static_cast<unsigned char>(191));
}

TEST(FramebufferTest, SetPixelColorOutOfBoundsDoesNothing) {
    Framebuffer fb({2, 2, 3});
    Eigen::Vector3d color(1.0, 1.0, 1.0);
    fb.setPixelColor(-1, 0, color, 1.0);
    fb.setPixelColor(2, 0, color, 1.0); 
    fb.setPixelColor(0, -1, color, 1.0);
    fb.setPixelColor(0, 2, color, 1.0);
    fb.generateImage();
    const unsigned char* image = fb.getImage();
    for (int i = 0; i < 2 * 2 * 3; ++i) {
        EXPECT_EQ(image[i], 0);
    }
}

TEST(FramebufferTest, DestructorFreesMemory) {
    Framebuffer* fb = new Framebuffer({10, 10, 3});
    fb->generateImage();
    const unsigned char* data = fb->getImage();
    EXPECT_NE(data, nullptr);
    delete fb;
}
