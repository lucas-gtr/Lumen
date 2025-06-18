#ifndef QT_VIEW_LIGHT_LIGHTVIEW_HPP
#define QT_VIEW_LIGHT_LIGHTVIEW_HPP

#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qwidget.h>

class LightView : public QWidget {
  Q_OBJECT

public:
  explicit LightView(QWidget* parent);

  void setIntensity(double value);

  void setColor(const QColor& color);

signals:
  void intensityChanged(double intensity);
  void colorChanged(const QColor& color);

protected:
  void addLayoutToContentLayout(QLayout* layout);
  void addGroupBoxToMainLayout(QGroupBox* group_box);

  void setupUI();
  void connectSignals();

private:
  // Slots
  void onColorButtonClicked();

  // Members
  QDoubleSpinBox* m_intensity_spin = nullptr;
  QPushButton*    m_color_button   = nullptr;

  QVBoxLayout* m_content_layout = nullptr;
  QVBoxLayout* m_main_layout    = nullptr;
};

#endif // QT_VIEW_LIGHT_LIGHTVIEW_HPP
