#include <cmath>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "GPU/Lights/SpotGPU.hpp"
#include "Lighting/SpotLight.hpp"

SpotLightGPU::SpotLightGPU(const SpotLight& light)
    : m_light(&light), m_position(lin::Vec3f(light.getPosition())), m_direction(lin::Vec3f(light.getDirection())),
      m_cos_inner_cutoff(static_cast<float>(std::cos(light.getInnerAngle()))),
      m_cos_outer_cutoff(static_cast<float>(std::cos(light.getOuterAngle()))) {

  const ColorRGB color = light.getColor();
  m_color = lin::Vec3f(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)) *
            static_cast<float>(light.getIntensity());
}