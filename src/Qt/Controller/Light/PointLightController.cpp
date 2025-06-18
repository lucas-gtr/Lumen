#include "Qt/Controller/Light/PointLightController.hpp"
#include "Lighting/PointLight.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/View/Light/PointLightView.hpp"

PointLightController::PointLightController(PointLightView* view) : LightController(view), m_point_light_view(view) {}

void PointLightController::setPointLight(PointLight* light) {
  LightController::setModel(light);
  m_point_light = light;
  updateViewFromModel();
}

void PointLightController::updateViewFromModel() {}
