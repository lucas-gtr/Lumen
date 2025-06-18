#include <qcolor.h>
#include <qobject.h>

#include "Core/CommonTypes.hpp"
#include "Lighting/Light.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/View/Light/LightView.hpp"

LightController::LightController(LightView* view) : QObject(view), m_view(view) { connectSignals(); }

void LightController::connectSignals() {
  connect(m_view, &LightView::intensityChanged, this, &LightController::onIntensityChanged);
  connect(m_view, &LightView::colorChanged, this, &LightController::onColorChanged);
}

void LightController::setModel(Light* light) {
  m_light = light;
  updateViewFromModel();
}

void LightController::onIntensityChanged(double value) {
  if(m_light != nullptr) {
    m_light->setIntensity(value);
  }
}

void LightController::onColorChanged(const QColor& color) {
  if(m_light != nullptr) {
    const ColorRGB new_color(color.redF(), color.greenF(), color.blueF());
    m_light->setColor(new_color);
  }
}

void LightController::updateViewFromModel() {
  if(m_light == nullptr) {
    return;
  }
  m_view->setIntensity(m_light->getIntensity());

  const ColorRGB color = m_light->getColor();
  m_view->setColor(
      QColor::fromRgbF(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)));
}
