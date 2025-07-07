#include <linalg/Vec3.hpp>

#include "Lighting/DirectionalLight.hpp"
#include "Qt/Controller/Light/DirectionalLightController.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/View/Light/DirectionalLightView.hpp"

DirectionalLightController::DirectionalLightController(DirectionalLightView* view)
    : LightController(view), m_directional_light_view(view) {
  connectSignals();
}

void DirectionalLightController::connectSignals() {
  connect(m_directional_light_view, &DirectionalLightView::directionChanged, this,
          &DirectionalLightController::onDirectionChanged);
}

void DirectionalLightController::onDirectionChanged(double x, double y, double z) {
  if(m_directional_light == nullptr) {
    return;
  }
  const linalg::Vec3d new_direction(x, y, z);
  m_directional_light->setDirection(new_direction);
}

void DirectionalLightController::setDirectionalLight(DirectionalLight* light) {
  LightController::setModel(light);
  m_directional_light = light;
  updateViewFromModel();
}

void DirectionalLightController::updateViewFromModel() {
  if(m_directional_light == nullptr) {
    return;
  }

  const linalg::Vec3d dir = m_directional_light->getDirection();
  m_directional_light_view->setDirection(dir.x, dir.y, dir.z);
}
