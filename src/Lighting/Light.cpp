#include "Lighting/Light.hpp"
#include "Core/CommonTypes.hpp"

void Light::setColor(const ColorRGB& color) {
  m_color = color;
  m_light_changed_observer.notify();
}

void Light::setIntensity(double intensity) {
  m_intensity = intensity;
  m_light_changed_observer.notify();
}