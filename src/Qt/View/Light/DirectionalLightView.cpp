#include "Qt/View/Light/DirectionalLightView.hpp"
#include "Qt/View/Light/LightView.hpp"

#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qwidget.h>

DirectionalLightView::DirectionalLightView(QWidget* parent) : LightView(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void DirectionalLightView::setupUI() {
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

  addLayoutToContentLayout(direction_layout);

  auto* group_box = new QGroupBox("Directional Light Settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");
  addGroupBoxToMainLayout(group_box);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void DirectionalLightView::connectSignals() {
  auto emitDirectionChanged = [this]() {
    const double x = m_direction_x->text().toDouble();
    const double y = m_direction_y->text().toDouble();
    const double z = m_direction_z->text().toDouble();
    emit         directionChanged(x, y, z);
  };

  connect(m_direction_x, &QLineEdit::editingFinished, this, emitDirectionChanged);
  connect(m_direction_y, &QLineEdit::editingFinished, this, emitDirectionChanged);
  connect(m_direction_z, &QLineEdit::editingFinished, this, emitDirectionChanged);
}

void DirectionalLightView::setDirection(double x, double y, double z) {
  const QSignalBlocker blocker_x(m_direction_x);
  const QSignalBlocker blocker_y(m_direction_y);
  const QSignalBlocker blocker_z(m_direction_z);

  m_direction_x->setText(QString::number(x));
  m_direction_y->setText(QString::number(y));
  m_direction_z->setText(QString::number(z));
}
