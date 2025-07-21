#include "TransformWidget.hpp"
#include "Core/Config.hpp"
#include "Core/Transform.hpp"
#include "ui_TransformWidget.h"

TransformWidget::TransformWidget(QWidget* parent) : QGroupBox(parent), ui(new Ui::TransformWidget) {
  ui->setupUi(this);

  setStyleSheet(QString::fromStdString(std::string(GROUP_BOX_STYLESHEET)));

  connect(ui->positionEditor, &Vec3Editor::valueChanged, this, &TransformWidget::onPositionChanged);
  connect(ui->rotationEditor, &Vec3Editor::valueChanged, this, &TransformWidget::onRotationChanged);
  connect(ui->scaleEditor, &Vec3Editor::valueChanged, this, &TransformWidget::onScaleChanged);
}

void TransformWidget::setTransform(Transform* transform) {
  m_transform = transform;
  if(m_transform == nullptr) {
    return;
  }
  updateWidget();
}

void TransformWidget::updateWidget() {
  if(m_transform == nullptr) {
    return;
  }

  ui->positionEditor->setValue(m_transform->getPosition());
  ui->rotationEditor->setValue(m_transform->getRotationDeg());
  ui->scaleEditor->setValue(m_transform->getScale());
}

void TransformWidget::onPositionChanged(const linalg::Vec3d& new_position) {
  if(m_transform != nullptr) {
    m_transform->setPosition(new_position);
  }
}

void TransformWidget::onRotationChanged(const linalg::Vec3d& new_rotation) {
  if(m_transform != nullptr) {
    m_transform->setRotationDeg(new_rotation);
  }
}

void TransformWidget::onScaleChanged(const linalg::Vec3d& new_scale) {
  if(m_transform != nullptr) {
    m_transform->setScale(new_scale);
  }
}

TransformWidget::~TransformWidget() { delete ui; }
