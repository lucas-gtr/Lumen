#ifndef QT_CONTROLLER_LIGHT_SPOTLIGHTCONTROLLER_HPP
#define QT_CONTROLLER_LIGHT_SPOTLIGHTCONTROLLER_HPP

#include "Lighting/SpotLight.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/View/Light/SpotLightView.hpp"

class SpotLightController : public LightController {
  Q_OBJECT

public:
  explicit SpotLightController(SpotLightView* view);

  void setSpotLight(SpotLight* light);

private:
  // Slots
  void onDirectionChanged(double x, double y, double z);
  void onInnerAngleChanged(double angle);
  void onOuterAngleChanged(double angle);

  // Members
  SpotLight*     m_spot_light = nullptr;
  SpotLightView* m_spot_light_view;

  void updateViewFromModel();
  void connectSignals();
};

#endif // QT_CONTROLLER_LIGHT_SPOTLIGHTCONTROLLER_HPP
