#ifndef QT_CONTROLLER_LIGHT_DIRECTIONALLIGHTCONTROLLER_HPP
#define QT_CONTROLLER_LIGHT_DIRECTIONALLIGHTCONTROLLER_HPP

#include "Lighting/DirectionalLight.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/View/Light/DirectionalLightView.hpp"

class DirectionalLightController : public LightController {
  Q_OBJECT

public:
  explicit DirectionalLightController(DirectionalLightView* view);

  void setDirectionalLight(DirectionalLight* light);

private:
  // Slots
  void onDirectionChanged(double x, double y, double z);

  // Members
  DirectionalLight*     m_directional_light = nullptr;
  DirectionalLightView* m_directional_light_view;

  void updateViewFromModel();
  void connectSignals();
};

#endif // QT_CONTROLLER_LIGHT_DIRECTIONALLIGHTCONTROLLER_HPP
