#include "GPU/OpenGL/Lights/DirectionalGL.hpp"
#include "Core/Config.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "GPU/Lights/DirectionalGPU.hpp"
#include "Lighting/DirectionalLight.hpp"

DirectionalLightGL::DirectionalLightGL(DirectionalLight* light) : DirectionalLightGPU(light) {
  updateLightSpaceMatrix();
  light->getTransformationChangedObserver().add([this]() { updateLightSpaceMatrix(); });
  light->getLightChangedObserver().add([this]() {
    retrieveData();
    updateLightSpaceMatrix();
  });
}

void DirectionalLightGL::updateLightSpaceMatrix() {
  const lin::Vec3f lightPos = -getDirection() * DIRECTIONAL_LIGHT_DISTANCE;

  const lin::Mat4f lightView = lin::Mat4f::LookAt(lightPos, lightPos + getDirection());
  const lin::Mat4f lightProjection =
      lin::Mat4f::Orthographic(-ORTHOGRAPHIC_LIGHT_SIZE, ORTHOGRAPHIC_LIGHT_SIZE, -ORTHOGRAPHIC_LIGHT_SIZE,
                               ORTHOGRAPHIC_LIGHT_SIZE, m_near_plane, m_far_plane);

  m_lightSpaceMatrix = lightProjection * lightView;
}