#include "GPU/Lights/DirectionalGPU.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Lighting/DirectionalLight.hpp"

DirectionalLightGPU::DirectionalLightGPU(const DirectionalLight& light)
    : m_light(&light), m_direction(lin::Vec3f(light.getDirection())) {
  const ColorRGB color = light.getColor();
  m_color = lin::Vec3f(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)) *
            static_cast<float>(light.getIntensity());
}