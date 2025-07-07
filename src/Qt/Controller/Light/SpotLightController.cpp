#include <linalg/Vec3.hpp>

#include "Core/MathConstants.hpp"
#include "Lighting/SpotLight.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/Controller/Light/SpotLightController.hpp"
#include "Qt/View/Light/SpotLightView.hpp"

SpotLightController::SpotLightController(SpotLightView* view) : LightController(view), m_spot_light_view(view) {
  connectSignals();
}

void SpotLightController::connectSignals() {
  connect(m_spot_light_view, &SpotLightView::directionChanged, this, &SpotLightController::onDirectionChanged);
  connect(m_spot_light_view, &SpotLightView::innerAngleChanged, this, &SpotLightController::onInnerAngleChanged);
  connect(m_spot_light_view, &SpotLightView::outerAngleChanged, this, &SpotLightController::onOuterAngleChanged);
}

void SpotLightController::onDirectionChanged(double x, double y, double z) {
  if(m_spot_light == nullptr) {
    return;
  }

  const linalg::Vec3d new_direction(x, y, z);
  m_spot_light->setDirection(new_direction);
}

void SpotLightController::onInnerAngleChanged(double angle) {
  if(m_spot_light == nullptr) {
    return;
  }

  m_spot_light->setInnerAngle(angle);
  updateViewFromModel();
}

void SpotLightController::onOuterAngleChanged(double angle) {
  if(m_spot_light == nullptr) {
    return;
  }

  m_spot_light->setOuterAngle(angle);
  updateViewFromModel();
}

void SpotLightController::setSpotLight(SpotLight* light) {
  LightController::setModel(light);
  m_spot_light = light;
  updateViewFromModel();
}

void SpotLightController::updateViewFromModel() {
  if(m_spot_light == nullptr) {
    return;
  }

  const linalg::Vec3d dir = m_spot_light->getDirection();
  m_spot_light_view->setDirection(dir.x, dir.y, dir.z);

  m_spot_light_view->setInnerAngle(m_spot_light->getInnerAngle() * RAD_TO_DEG);
  m_spot_light_view->setOuterAngle(m_spot_light->getOuterAngle() * RAD_TO_DEG);
}
