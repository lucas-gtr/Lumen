#include <linalg/Mat4.hpp>
#include <linalg/Vec3.hpp>

#include "Core/Config.hpp"
#include "GPU/Lights/DirectionalGPU.hpp"
#include "GPU/OpenGL/Lights/DirectionalGL.hpp"
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
  const linalg::Vec3f light_pos = -getDirection() * DIRECTIONAL_LIGHT_DISTANCE;

  const linalg::Mat4f light_view = linalg::Mat4f::LookAt(light_pos, light_pos + getDirection());
  const linalg::Mat4f light_projection =
      linalg::Mat4f::Orthographic(-ORTHOGRAPHIC_LIGHT_SIZE, ORTHOGRAPHIC_LIGHT_SIZE, -ORTHOGRAPHIC_LIGHT_SIZE,
                                  ORTHOGRAPHIC_LIGHT_SIZE, m_near_plane, m_far_plane);

  m_light_space_matrix = light_projection * light_view;
}