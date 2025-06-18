#include <gtest/gtest.h>
#include "GPU/IResourceManagerGPU.hpp"

class TestResourceManagerGPU : public IResourceManagerGPU {
public:
    TestResourceManagerGPU() : IResourceManagerGPU() {}

    void setCamera(Camera* camera, int viewportWidth, int viewportHeight) override {}

    const ITextureGPU* addTexture(Texture* texture) override {
        return nullptr;
    }

    void addObject3D(Object3D* object) override {}

    void addLight(DirectionalLight* light) override {}

    void addLight(PointLight* light) override {}

    void addLight(SpotLight* light) override {}

    void clearAllRessources() override {}

    void setSkybox(Skybox* skybox) override {}
};

TEST(IResourceManagerGPUTest, AddObject3D) {
    TestResourceManagerGPU resourceManager;
    Object3D object;
    object.setPosition(lin::Vec3d(1.0, 2.0, 3.0));
    object.setRotation(lin::Vec3d(45.0, 90.0, 180.0));

    EXPECT_NO_THROW(resourceManager.addObject3D(&object));
}