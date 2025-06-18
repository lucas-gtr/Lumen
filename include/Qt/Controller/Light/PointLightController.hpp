#ifndef QT_CONTROLLER_LIGHT_POINTLIGHTCONTROLLER_HPP
#define QT_CONTROLLER_LIGHT_POINTLIGHTCONTROLLER_HPP

#include "Lighting/PointLight.hpp"
#include "Qt/Controller/Light/LightController.hpp"
#include "Qt/View/Light/PointLightView.hpp"

class PointLightController : public LightController {
  Q_OBJECT

public:
  explicit PointLightController(PointLightView* view);

  void setPointLight(PointLight* light);

private:
  PointLight*     m_point_light = nullptr;
  PointLightView* m_point_light_view;

  void updateViewFromModel();
};

#endif // QT_CONTROLLER_LIGHT_POINTLIGHTCONTROLLER_HPP
