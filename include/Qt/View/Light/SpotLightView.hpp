#ifndef QT_VIEW_LIGHT_SPOTLIGHTVIEW_HPP
#define QT_VIEW_LIGHT_SPOTLIGHTVIEW_HPP

#include "Qt/View/Light/LightView.hpp"
#include <qlineedit.h>
#include <qspinbox.h>

class SpotLightView : public LightView {
  Q_OBJECT

public:
  explicit SpotLightView(QWidget* parent);

  void setDirection(double x, double y, double z);
  void setInnerAngle(double angle);
  void setOuterAngle(double angle);

signals:
  void directionChanged(double x, double y, double z);
  void innerAngleChanged(double angle);
  void outerAngleChanged(double angle);

private:
  // Slots
  void onInnerAngleChanged();
  void onOuterAngleChanged();

  QLineEdit* m_direction_x = nullptr;
  QLineEdit* m_direction_y = nullptr;
  QLineEdit* m_direction_z = nullptr;

  QDoubleSpinBox* m_inner_angle_spin = nullptr;
  QDoubleSpinBox* m_outer_angle_spin = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_LIGHT_SPOTLIGHTVIEW_HPP
