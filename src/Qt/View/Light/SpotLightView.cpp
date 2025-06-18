#include <qboxlayout.h>
#include <qglobal.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qobject.h>
#include <qspinbox.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include "Core/MathConstants.hpp"
#include "Qt/View/Light/LightView.hpp"
#include "Qt/View/Light/SpotLightView.hpp"

SpotLightView::SpotLightView(QWidget* parent) : LightView(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void SpotLightView::setupUI() {
  auto* direction_label = new QLabel("Direction", this);

  m_direction_x = new QLineEdit(this);
  m_direction_y = new QLineEdit(this);
  m_direction_z = new QLineEdit(this);

  auto* direction_inputs_layout = new QHBoxLayout;
  direction_inputs_layout->addWidget(new QLabel("X ", this));
  direction_inputs_layout->addWidget(m_direction_x);
  direction_inputs_layout->addWidget(new QLabel("Y ", this));
  direction_inputs_layout->addWidget(m_direction_y);
  direction_inputs_layout->addWidget(new QLabel("Z ", this));
  direction_inputs_layout->addWidget(m_direction_z);

  auto* direction_layout = new QVBoxLayout;
  direction_layout->addWidget(direction_label);
  direction_layout->addLayout(direction_inputs_layout);

  auto* angles_label = new QLabel("Spotlight Angles", this);

  m_inner_angle_spin = new QDoubleSpinBox(this);
  m_inner_angle_spin->setRange(0.0, ANGLE_MAX_DEGREES);
  m_inner_angle_spin->setSingleStep(1.0);
  m_inner_angle_spin->setSuffix("°");

  m_outer_angle_spin = new QDoubleSpinBox(this);
  m_outer_angle_spin->setRange(0.0, ANGLE_MAX_DEGREES);
  m_outer_angle_spin->setSingleStep(1.0);
  m_outer_angle_spin->setSuffix("°");

  auto* angles_layout = new QHBoxLayout;
  angles_layout->addWidget(new QLabel("Inner Angle ", this));
  angles_layout->addWidget(m_inner_angle_spin);
  angles_layout->addStretch();
  angles_layout->addWidget(new QLabel("Outer Angle ", this));
  angles_layout->addWidget(m_outer_angle_spin);

  auto* angles_input_layout = new QVBoxLayout;
  angles_input_layout->addWidget(angles_label);
  angles_input_layout->addLayout(angles_layout);

  addLayoutToContentLayout(direction_layout);
  addLayoutToContentLayout(angles_input_layout);

  auto* group_box = new QGroupBox("Spot Light Settings");
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");
  addGroupBoxToMainLayout(group_box);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void SpotLightView::connectSignals() {
  auto emitDirectionChanged = [this]() {
    const double x = m_direction_x->text().toDouble();
    const double y = m_direction_y->text().toDouble();
    const double z = m_direction_z->text().toDouble();
    emit         directionChanged(x, y, z);
  };

  connect(m_direction_x, &QLineEdit::editingFinished, this, emitDirectionChanged);
  connect(m_direction_y, &QLineEdit::editingFinished, this, emitDirectionChanged);
  connect(m_direction_z, &QLineEdit::editingFinished, this, emitDirectionChanged);

  connect(m_inner_angle_spin, &QDoubleSpinBox::editingFinished, this, &SpotLightView::onInnerAngleChanged);
  connect(m_outer_angle_spin, &QDoubleSpinBox::editingFinished, this, &SpotLightView::onOuterAngleChanged);
}

void SpotLightView::onInnerAngleChanged() {
  const double angle = m_inner_angle_spin->value();
  emit         innerAngleChanged(angle);
}

void SpotLightView::onOuterAngleChanged() {
  const double angle = m_outer_angle_spin->value();
  emit         outerAngleChanged(angle);
}

void SpotLightView::setDirection(double x, double y, double z) {
  const QSignalBlocker blocker_x(m_direction_x);
  const QSignalBlocker blocker_y(m_direction_y);
  const QSignalBlocker blocker_z(m_direction_z);

  m_direction_x->setText(QString::number(x));
  m_direction_y->setText(QString::number(y));
  m_direction_z->setText(QString::number(z));
}

void SpotLightView::setInnerAngle(double angle) {
  const QSignalBlocker blocker(m_inner_angle_spin);
  m_inner_angle_spin->setValue(angle);
}

void SpotLightView::setOuterAngle(double angle) {
  const QSignalBlocker blocker(m_outer_angle_spin);
  m_outer_angle_spin->setValue(angle);
}
