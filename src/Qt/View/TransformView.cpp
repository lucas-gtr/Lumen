#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qvalidator.h>
#include <qwidget.h>

#include "Qt/View/TransformView.hpp"

TransformView::TransformView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void TransformView::setupUI() {
  auto* main_layout = new QVBoxLayout;
  auto* validator   = new QDoubleValidator(this);

  auto* layout = new QVBoxLayout;

  layout->addWidget(new QLabel("Position", this));

  m_position_x = new QLineEdit(this);
  m_position_x->setValidator(validator);
  m_position_x->setAlignment(Qt::AlignRight);

  m_position_y = new QLineEdit(this);
  m_position_y->setValidator(validator);
  m_position_y->setAlignment(Qt::AlignRight);

  m_position_z = new QLineEdit(this);
  m_position_z->setValidator(validator);
  m_position_z->setAlignment(Qt::AlignRight);

  auto* position_layout = new QHBoxLayout;
  position_layout->addWidget(new QLabel("X", this));
  position_layout->addWidget(m_position_x);
  position_layout->addWidget(new QLabel("Y", this));
  position_layout->addWidget(m_position_y);
  position_layout->addWidget(new QLabel("Z", this));
  position_layout->addWidget(m_position_z);
  layout->addLayout(position_layout);

  layout->addWidget(new QLabel("Rotation", this));

  m_rotation_x = new QLineEdit(this);
  m_rotation_x->setValidator(validator);
  m_rotation_x->setAlignment(Qt::AlignRight);

  m_rotation_y = new QLineEdit(this);
  m_rotation_y->setValidator(validator);
  m_rotation_y->setAlignment(Qt::AlignRight);

  m_rotation_z = new QLineEdit(this);
  m_rotation_z->setValidator(validator);
  m_rotation_z->setAlignment(Qt::AlignRight);

  auto* rotation_layout = new QHBoxLayout;
  rotation_layout->addWidget(new QLabel("X", this));
  rotation_layout->addWidget(m_rotation_x);
  rotation_layout->addWidget(new QLabel("Y", this));
  rotation_layout->addWidget(m_rotation_y);
  rotation_layout->addWidget(new QLabel("Z", this));
  rotation_layout->addWidget(m_rotation_z);
  layout->addLayout(rotation_layout);

  layout->addWidget(new QLabel("Scale", this));

  m_scale_x = new QLineEdit(this);
  m_scale_x->setValidator(validator);
  m_scale_x->setAlignment(Qt::AlignRight);

  m_scale_y = new QLineEdit(this);
  m_scale_y->setValidator(validator);
  m_scale_y->setAlignment(Qt::AlignRight);

  m_scale_z = new QLineEdit(this);
  m_scale_z->setValidator(validator);
  m_scale_z->setAlignment(Qt::AlignRight);

  auto* scale_layout = new QHBoxLayout;
  scale_layout->addWidget(new QLabel("X", this));
  scale_layout->addWidget(m_scale_x);
  scale_layout->addWidget(new QLabel("Y", this));
  scale_layout->addWidget(m_scale_y);
  scale_layout->addWidget(new QLabel("Z", this));
  scale_layout->addWidget(m_scale_z);
  layout->addLayout(scale_layout);

  auto* group_box = new QGroupBox("Transform Settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");
  group_box->setLayout(layout);

  main_layout->addWidget(group_box);
  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void TransformView::connectSignals() {
  connect(m_position_x, &QLineEdit::textChanged, this, &TransformView::positionXChanged);
  connect(m_position_y, &QLineEdit::textChanged, this, &TransformView::positionYChanged);
  connect(m_position_z, &QLineEdit::textChanged, this, &TransformView::positionZChanged);

  connect(m_rotation_x, &QLineEdit::textChanged, this, &TransformView::rotationXChanged);
  connect(m_rotation_y, &QLineEdit::textChanged, this, &TransformView::rotationYChanged);
  connect(m_rotation_z, &QLineEdit::textChanged, this, &TransformView::rotationZChanged);

  connect(m_scale_x, &QLineEdit::textChanged, this, &TransformView::scaleXChanged);
  connect(m_scale_y, &QLineEdit::textChanged, this, &TransformView::scaleYChanged);
  connect(m_scale_z, &QLineEdit::textChanged, this, &TransformView::scaleZChanged);
}

void TransformView::SetLineEditValue(QLineEdit* line_edit, double value) { line_edit->setText(QString::number(value)); }

void TransformView::setPositionX(double value) {
  if(m_position_x->isEnabled()) {
    const QSignalBlocker blocker(m_position_x);
    SetLineEditValue(m_position_x, value);
  }
}

void TransformView::setPositionY(double value) {
  if(m_position_y->isEnabled()) {
    const QSignalBlocker blocker(m_position_y);
    SetLineEditValue(m_position_y, value);
  }
}

void TransformView::setPositionZ(double value) {
  if(m_position_z->isEnabled()) {
    const QSignalBlocker blocker(m_position_z);
    SetLineEditValue(m_position_z, value);
  }
}

void TransformView::setRotationX(double value) {
  if(m_rotation_x->isEnabled()) {
    const QSignalBlocker blocker(m_rotation_x);
    SetLineEditValue(m_rotation_x, value);
  }
}

void TransformView::setRotationY(double value) {
  if(m_rotation_y->isEnabled()) {
    const QSignalBlocker blocker(m_rotation_y);
    SetLineEditValue(m_rotation_y, value);
  }
}

void TransformView::setRotationZ(double value) {
  if(m_rotation_z->isEnabled()) {
    const QSignalBlocker blocker(m_rotation_z);
    SetLineEditValue(m_rotation_z, value);
  }
}

void TransformView::setScaleX(double value) {
  if(m_scale_x->isEnabled()) {
    const QSignalBlocker blocker(m_scale_x);
    SetLineEditValue(m_scale_x, value);
  }
}

void TransformView::setScaleY(double value) {
  if(m_scale_y->isEnabled()) {
    const QSignalBlocker blocker(m_scale_y);
    SetLineEditValue(m_scale_y, value);
  }
}

void TransformView::setScaleZ(double value) {
  if(m_scale_z->isEnabled()) {
    const QSignalBlocker blocker(m_scale_z);
    SetLineEditValue(m_scale_z, value);
  }
}

void TransformView::enablePositionInputs() {
  m_position_x->setEnabled(true);
  m_position_y->setEnabled(true);
  m_position_z->setEnabled(true);
}

void TransformView::enableRotationInputs() {
  m_rotation_x->setEnabled(true);
  m_rotation_y->setEnabled(true);
  m_rotation_z->setEnabled(true);
}

void TransformView::enableScaleInputs() {
  m_scale_x->setEnabled(true);
  m_scale_y->setEnabled(true);
  m_scale_z->setEnabled(true);
}

void TransformView::disableAllInputs() {
  m_position_x->setEnabled(false);
  m_position_y->setEnabled(false);
  m_position_z->setEnabled(false);
  m_rotation_x->setEnabled(false);
  m_rotation_y->setEnabled(false);
  m_rotation_z->setEnabled(false);
  m_scale_x->setEnabled(false);
  m_scale_y->setEnabled(false);
  m_scale_z->setEnabled(false);

  const QSignalBlocker blocker_x(m_position_x);
  const QSignalBlocker blocker_y(m_position_y);
  const QSignalBlocker blocker_z(m_position_z);
  const QSignalBlocker blocker_rot_x(m_rotation_x);
  const QSignalBlocker blocker_rot_y(m_rotation_y);
  const QSignalBlocker blocker_rot_z(m_rotation_z);
  const QSignalBlocker blocker_scale_x(m_scale_x);
  const QSignalBlocker blocker_scale_y(m_scale_y);
  const QSignalBlocker blocker_scale_z(m_scale_z);

  m_position_x->clear();
  m_position_y->clear();
  m_position_z->clear();
  m_rotation_x->clear();
  m_rotation_y->clear();
  m_rotation_z->clear();
  m_scale_x->clear();
  m_scale_y->clear();
  m_scale_z->clear();
}
