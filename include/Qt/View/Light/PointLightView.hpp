#ifndef QT_VIEW_LIGHT_POINTLIGHTVIEW_HPP
#define QT_VIEW_LIGHT_POINTLIGHTVIEW_HPP

#include "Qt/View/Light/LightView.hpp"
#include <qlineedit.h>

class PointLightView : public LightView {
  Q_OBJECT

public:
  explicit PointLightView(QWidget* parent);

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_LIGHT_POINTLIGHTVIEW_HPP
