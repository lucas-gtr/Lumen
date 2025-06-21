#include "Qt/View/Light/PointLightView.hpp"
#include "Qt/View/Light/LightView.hpp"

#include <qgroupbox.h>
#include <qwidget.h>

PointLightView::PointLightView(QWidget* parent) : LightView(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void PointLightView::setupUI() {
  auto* group_box = new QGroupBox("Point Light Settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");
  addGroupBoxToMainLayout(group_box);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void PointLightView::connectSignals() {}
