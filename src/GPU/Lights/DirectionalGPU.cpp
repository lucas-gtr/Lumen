#include <linalg/Vec3.hpp>

#include "Core/CommonTypes.hpp"
#include "GPU/Lights/DirectionalGPU.hpp"
#include "Lighting/DirectionalLight.hpp"

DirectionalLightGPU::DirectionalLightGPU(const DirectionalLight* light) : m_light(light) { retrieveData(); }

void DirectionalLightGPU::retrieveData() {
  m_direction          = linalg::Vec3f(m_light->getDirection());
  const ColorRGB color = m_light->getColor();
  m_color = linalg::Vec3f(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)) *
            static_cast<float>(m_light->getIntensity());
}