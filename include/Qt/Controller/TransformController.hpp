#ifndef QT_CONTROLLER_TRANSFORMCONTROLLER_HPP
#define QT_CONTROLLER_TRANSFORMCONTROLLER_HPP

#include "Core/Transform.hpp"
#include "Qt/View/TransformView.hpp"
#include <qobject.h>

class TransformController : public QObject {
  Q_OBJECT

public:
  explicit TransformController(TransformView* view);
  void setTransform(Transform* model);

  void updateViewFromModel();

private:
  // Slots
  void onPositionXChanged(const QString& txt);
  void onPositionYChanged(const QString& txt);
  void onPositionZChanged(const QString& txt);
  void onRotationXChanged(const QString& txt);
  void onRotationYChanged(const QString& txt);
  void onRotationZChanged(const QString& txt);
  void onScaleXChanged(const QString& txt);
  void onScaleYChanged(const QString& txt);
  void onScaleZChanged(const QString& txt);

  // Members
  Transform*     m_transform = nullptr;
  TransformView* m_view;

  void connectSignals();
};

#endif // QT_CONTROLLER_TRANSFORMCONTROLLER_HPP
