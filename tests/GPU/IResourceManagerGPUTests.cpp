#include <gtest/gtest.h>
#include "GPU/IResourceManagerGPU.hpp"

class TestResourceManagerGPU : public IResourceManagerGPU {
public:
    TestResourceManagerGPU() : IResourceManagerGPU() {}

    void setCamera(Camera* camera, int viewportWidth, int viewportHeight) override {}

    const ITextureGPU* addTexture(const Texture& texture) override {
        return nullptr;
    }

    void addObject3D(const Object3D& object) override {}

    void addLight(const DirectionalLight& light) override {}

    void addLight(const PointLight& light) override {}

    void addLight(const SpotLight& light) override {}

    void clearAllRessources() override {}

    void setSkyboxTexture(const Texture& texture) override {}

    void deleteSkyboxTexture() override {}
};

TEST(IResourceManagerGPUTest, AddObject3D) {
    TestResourceManagerGPU resourceManager;
    Object3D object;
    object.setPosition(lin::Vec3d(1.0, 2.0, 3.0));
    object.setRotation(lin::Vec3d(45.0, 90.0, 180.0));

    EXPECT_NO_THROW(resourceManager.addObject3D(object));
}