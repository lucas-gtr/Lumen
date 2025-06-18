#ifndef QT_CONTROLLER_LIGHT_LIGHTCONTROLLER_HPP
#define QT_CONTROLLER_LIGHT_LIGHTCONTROLLER_HPP

#include "Lighting/Light.hpp"
#include "Qt/View/Light/LightView.hpp"
#include <qcolor.h>
#include <qobject.h>

class LightController : public QObject {
  Q_OBJECT

public:
  explicit LightController(LightView* view);

  void setModel(Light* light);

protected:
  void updateViewFromModel();
  void connectSignals();

private:
  // Slots
  void onIntensityChanged(double value);
  void onColorChanged(const QColor& color);

  // Members
  Light*     m_light = nullptr;
  LightView* m_view;
};

#endif // QT_CONTROLLER_LIGHT_LIGHTCONTROLLER_HPP
