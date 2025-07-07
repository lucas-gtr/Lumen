#ifndef QT_VIEW_TRANSFORMVIEW_HPP
#define QT_VIEW_TRANSFORMVIEW_HPP

#include <qboxlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qwidget.h>

class TransformView : public QWidget {
  Q_OBJECT

public:
  explicit TransformView(QWidget* parent);

  void setPositionX(double value);
  void setPositionY(double value);
  void setPositionZ(double value);

  void setRotationX(double value);
  void setRotationY(double value);
  void setRotationZ(double value);

  void setScaleX(double value);
  void setScaleY(double value);
  void setScaleZ(double value);

  void enablePositionInputs();
  void enableRotationInputs();
  void enableScaleInputs();

  void disableAllInputs();

  static void SetLineEditValue(QLineEdit* line_edit, double value);

signals:
  void positionXChanged(const QString& position_x);
  void positionYChanged(const QString& position_y);
  void positionZChanged(const QString& position_z);
  void rotationXChanged(const QString& rotation_x);
  void rotationYChanged(const QString& rotation_y);
  void rotationZChanged(const QString& rotation_z);
  void scaleXChanged(const QString& scale_x);
  void scaleYChanged(const QString& scale_y);
  void scaleZChanged(const QString& scale_z);

private:
  QLineEdit* m_position_x = nullptr;
  QLineEdit* m_position_y = nullptr;
  QLineEdit* m_position_z = nullptr;
  QLineEdit* m_rotation_x = nullptr;
  QLineEdit* m_rotation_y = nullptr;
  QLineEdit* m_rotation_z = nullptr;
  QLineEdit* m_scale_x    = nullptr;
  QLineEdit* m_scale_y    = nullptr;
  QLineEdit* m_scale_z    = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_TRANSFORMVIEW_HPP