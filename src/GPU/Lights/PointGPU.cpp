#include "GPU/Lights/PointGPU.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Lighting/PointLight.hpp"

PointLightGPU::PointLightGPU(const PointLight* light) : m_light(light) { retrieveData(); }

void PointLightGPU::retrieveData() {
  m_position           = lin::Vec3f(m_light->getPosition());
  const ColorRGB color = m_light->getColor();
  m_color = lin::Vec3f(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)) *
            static_cast<float>(m_light->getIntensity());
}