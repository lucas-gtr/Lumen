#include <linalg/Vec3.hpp>

#include "Core/Color.hpp"
#include "GPU/Lights/PointGPU.hpp"
#include "Lighting/PointLight.hpp"

PointLightGPU::PointLightGPU(const PointLight* light) : m_light(light) { PointLightGPU::retrieveData(); }

void PointLightGPU::retrieveData() {
  m_position           = linalg::Vec3f(m_light->getPosition());
  const ColorRGB color = m_light->getColor();
  setColor(linalg::Vec3f(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)) *
           static_cast<float>(m_light->getIntensity()));
}