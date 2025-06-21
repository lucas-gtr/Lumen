#include <qboxlayout.h>
#include <qcolordialog.h>
#include <qglobal.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/View/Light/LightView.hpp"

LightView::LightView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void LightView::setupUI() {
  m_main_layout = new QVBoxLayout;

  m_intensity_spin = new QDoubleSpinBox(this);
  m_intensity_spin->setRange(0.0, MAX_LIGHT_INTENSITY);
  m_intensity_spin->setSingleStep(1.0);
  m_intensity_spin->setValue(1.0);

  m_color_button = new QPushButton(this);
  m_color_button->setFixedSize(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT);
  m_color_button->setStyleSheet("background-color: rgb(255,255,255);");

  m_content_layout = new QVBoxLayout;

  auto* intensity_layout = new QHBoxLayout;
  intensity_layout->addWidget(new QLabel("Intensity ", this));
  intensity_layout->addWidget(m_intensity_spin);
  m_content_layout->addLayout(intensity_layout);

  auto* color_layout = new QHBoxLayout;
  color_layout->addWidget(new QLabel("Color ", this));
  color_layout->addWidget(m_color_button);
  m_content_layout->addLayout(color_layout);
  setLayout(m_main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void LightView::connectSignals() {
  connect(m_intensity_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &LightView::intensityChanged);
  connect(m_color_button, &QPushButton::clicked, this, &LightView::onColorButtonClicked);
}

void LightView::addLayoutToContentLayout(QLayout* layout) { m_content_layout->addLayout(layout); }

void LightView::addGroupBoxToMainLayout(QGroupBox* group_box) {
  group_box->setLayout(m_content_layout);
  m_main_layout->addWidget(group_box);
}

void LightView::onColorButtonClicked() {
  const QColor color = QColorDialog::getColor(Qt::white, this, "Select Light Color");
  if(color.isValid()) {
    setColor(color);
    emit colorChanged(color);
  }
}

void LightView::setIntensity(double value) {
  const QSignalBlocker blocker(m_intensity_spin);
  m_intensity_spin->setValue(value);
}

void LightView::setColor(const QColor& color) {
  const QSignalBlocker blocker(m_color_button);
  m_color_button->setStyleSheet(QString("background-color: %1;").arg(color.name()));
}
