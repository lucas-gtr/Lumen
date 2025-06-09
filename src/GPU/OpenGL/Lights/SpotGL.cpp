#include <cmath>

#include "Core/Config.hpp"
#include "Core/Math/Mat4.hpp"
#include "GPU/Lights/SpotGPU.hpp"
#include "GPU/OpenGL/Lights/SpotGL.hpp"
#include "Lighting/SpotLight.hpp"

SpotLightGL::SpotLightGL(const SpotLight& light) : SpotLightGPU(light) { updateLightSpaceMatrix(); }

void SpotLightGL::updateLightSpaceMatrix() {
  m_far_plane = static_cast<float>(std::sqrt(light()->getIntensity() / MIN_LIGHT_INTENSITY_FAR_PLANE));

  const lin::Mat4f lightView =
      lin::Mat4f::LookAt(getPosition(), getPosition() + getDirection(), lin::Vec3f(0.0F, 1.0F, 0.0F));
  const lin::Mat4f lightProjection =
      lin::Mat4f::Perspective(static_cast<float>(light()->getOuterAngle()), 1.0F, m_near_plane, m_far_plane);

  m_lightSpaceMatrix = lightProjection * lightView;
}