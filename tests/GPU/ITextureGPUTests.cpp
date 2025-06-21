#include <gtest/gtest.h>
#include "GPU/ITextureGPU.hpp"
#include "Surface/Texture.hpp"

// Implémentation minimale pour permettre le test
class TestTextureGPU : public ITextureGPU {
public:
  explicit TestTextureGPU(Texture* texture) : ITextureGPU(texture) {}

  void uploadToGPU() override {  }

  void release() override {  }
};

// Tests
TEST(ITextureGPUTest, ConstructorStoresTexturePointer) {
  Texture texture;
  texture.setValue(1.0);

  TestTextureGPU gpuTexture(&texture);

  EXPECT_EQ(gpuTexture.getSource(), &texture);
}