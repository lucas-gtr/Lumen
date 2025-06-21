#ifndef QT_CONTROLLER_VIEWPORTCONTROLLER_HPP
#define QT_CONTROLLER_VIEWPORTCONTROLLER_HPP

#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/View/ViewportView.hpp"

#include <qobject.h>

class ViewportController : public QObject {
  Q_OBJECT

public:
  explicit ViewportController(ViewportView* view, RealtimeController* realtime_controller);

  void updateViewFromModel();

private:
  // Slots
  void onRotationSpeedChanged(double speed);
  void onZoomSpeedChanged(double speed);
  void onMoveSpeedChanged(double speed);
  void onShadowMapSizeChanged(int size);
  void onDynamicLightingChanged(bool enabled);
  void onDynamicShadowChanged(bool enabled);
  void onBakeLights();
  void onToneMappingChanged(const QString& mode);
  void onExposureChanged(double exposure);

  // Members
  ViewportView*       m_view;
  RealtimeController* m_realtime_controller;

  void connectSignals();
};

#endif // QT_CONTROLLER_VIEWPORTCONTROLLER_HPP
