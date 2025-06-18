#ifndef QT_VIEW_LIGHT_DIRECTIONALLIGHTVIEW_HPP
#define QT_VIEW_LIGHT_DIRECTIONALLIGHTVIEW_HPP

#include "Qt/View/Light/LightView.hpp"
#include <qlineedit.h>

class DirectionalLightView : public LightView {
  Q_OBJECT

public:
  explicit DirectionalLightView(QWidget* parent);

  void setDirection(double x, double y, double z);

signals:
  void directionChanged(double x, double y, double z);

private:
  QLineEdit* m_direction_x = nullptr;
  QLineEdit* m_direction_y = nullptr;
  QLineEdit* m_direction_z = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_LIGHT_DIRECTIONALLIGHTVIEW_HPP
