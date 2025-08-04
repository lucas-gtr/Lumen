#include "Lighting/Light.hpp"
#include "Core/Color.hpp"

void Light::setColor(const ColorRGB& color) {
  m_color = color;
  m_light_changed_observer.notify();
}

void Light::setIntensity(double intensity) {
  m_intensity = intensity;
  m_light_changed_observer.notify();
}

Light::~Light() {
  m_light_deleted_observer.notify(this);
  m_light_changed_observer.clear();
  m_light_deleted_observer.clear();
}