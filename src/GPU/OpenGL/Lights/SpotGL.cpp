#include <cmath>
#include <linalg/Mat4.hpp>

#include "Core/Config.hpp"
#include "GPU/Lights/SpotGPU.hpp"
#include "GPU/OpenGL/Lights/SpotGL.hpp"
#include "Lighting/SpotLight.hpp"

SpotLightGL::SpotLightGL(SpotLight* light) : SpotLightGPU(light) {
  updateLightSpaceMatrix();
  light->getTransformationChangedObserver().add([this]() {
    retrieveData();
    updateLightSpaceMatrix();
  });
  light->getLightChangedObserver().add([this]() {
    retrieveData();
    updateLightSpaceMatrix();
  });
}

void SpotLightGL::updateLightSpaceMatrix() {
  m_far_plane = static_cast<float>(std::sqrt(light()->getIntensity() / MIN_LIGHT_INTENSITY_FAR_PLANE));

  const linalg::Mat4f light_view =
      linalg::Mat4f::LookAt(getPosition(), getPosition() + getDirection(), linalg::Vec3f(0.0F, 1.0F, 0.0F));
  const linalg::Mat4f light_projection =
      linalg::Mat4f::Perspective(static_cast<float>(light()->getOuterAngle()), 1.0F, m_near_plane, m_far_plane);

  m_light_space_matrix = light_projection * light_view;
}