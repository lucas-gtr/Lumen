#include <qobject.h>

#include "Core/MathConstants.hpp"
#include "Core/Transform.hpp"
#include "Qt/Controller/TransformController.hpp"
#include "Qt/View/TransformView.hpp"

TransformController::TransformController(TransformView* view) : QObject(view), m_view(view) {
  connectSignals();
  updateViewFromModel();
}

void TransformController::connectSignals() {
  connect(m_view, &TransformView::positionXChanged, this, &TransformController::onPositionXChanged);
  connect(m_view, &TransformView::positionYChanged, this, &TransformController::onPositionYChanged);
  connect(m_view, &TransformView::positionZChanged, this, &TransformController::onPositionZChanged);
  connect(m_view, &TransformView::rotationXChanged, this, &TransformController::onRotationXChanged);
  connect(m_view, &TransformView::rotationYChanged, this, &TransformController::onRotationYChanged);
  connect(m_view, &TransformView::rotationZChanged, this, &TransformController::onRotationZChanged);
  connect(m_view, &TransformView::scaleXChanged, this, &TransformController::onScaleXChanged);
  connect(m_view, &TransformView::scaleYChanged, this, &TransformController::onScaleYChanged);
  connect(m_view, &TransformView::scaleZChanged, this, &TransformController::onScaleZChanged);
}

void TransformController::setTransform(Transform* model) {
  m_transform = model;
  updateViewFromModel();
}

void TransformController::updateViewFromModel() {
  if(m_transform == nullptr) {
    return; // No model set, nothing to update
  }
  m_view->setPositionX(m_transform->getPosition().x);
  m_view->setPositionY(m_transform->getPosition().y);
  m_view->setPositionZ(m_transform->getPosition().z);
  m_view->setRotationX(m_transform->getRotation().x * RAD_TO_DEG);
  m_view->setRotationY(m_transform->getRotation().y * RAD_TO_DEG);
  m_view->setRotationZ(m_transform->getRotation().z * RAD_TO_DEG);
  m_view->setScaleX(m_transform->getScale().x);
  m_view->setScaleY(m_transform->getScale().y);
  m_view->setScaleZ(m_transform->getScale().z);
}

void TransformController::onPositionXChanged(const QString& txt) { m_transform->setPositionX(txt.toDouble()); }

void TransformController::onPositionYChanged(const QString& txt) { m_transform->setPositionY(txt.toDouble()); }

void TransformController::onPositionZChanged(const QString& txt) { m_transform->setPositionZ(txt.toDouble()); }

void TransformController::onRotationXChanged(const QString& txt) { m_transform->setRotationX(txt.toDouble()); }

void TransformController::onRotationYChanged(const QString& txt) { m_transform->setRotationY(txt.toDouble()); }

void TransformController::onRotationZChanged(const QString& txt) { m_transform->setRotationZ(txt.toDouble()); }

void TransformController::onScaleXChanged(const QString& txt) { m_transform->setScaleX(txt.toDouble()); }

void TransformController::onScaleYChanged(const QString& txt) { m_transform->setScaleY(txt.toDouble()); }

void TransformController::onScaleZChanged(const QString& txt) { m_transform->setScaleZ(txt.toDouble()); }
