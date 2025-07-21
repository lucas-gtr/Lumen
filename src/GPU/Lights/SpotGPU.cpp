#include <cmath>
#include <linalg/Vec3.hpp>

#include "Core/CommonTypes.hpp"
#include "GPU/Lights/SpotGPU.hpp"
#include "Lighting/SpotLight.hpp"

SpotLightGPU::SpotLightGPU(const SpotLight* light) : m_light(light) { SpotLightGPU::retrieveData(); }

void SpotLightGPU::retrieveData() {
  m_position         = linalg::Vec3f(m_light->getPosition());
  m_direction        = linalg::Vec3f(m_light->getDirection());
  m_cos_inner_cutoff = static_cast<float>(std::cos(m_light->getInnerAngleRad()));
  m_cos_outer_cutoff = static_cast<float>(std::cos(m_light->getOuterAngleRad()));

  const ColorRGB color = m_light->getColor();
  setColor(linalg::Vec3f(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)) *
           static_cast<float>(m_light->getIntensity()));
}