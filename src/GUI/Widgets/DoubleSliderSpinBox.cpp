#include "DoubleSliderSpinBox.hpp"
#include "Core/Config.hpp"
#include "ui_DoubleSliderSpinBox.h"

#include <QSlider>
#include <QSpinBox>

DoubleSliderSpinBox::DoubleSliderSpinBox(QWidget* parent) : QWidget(parent), ui(new Ui::DoubleSliderSpinBox) {
  ui->setupUi(this);
  setupConnections();
}

void DoubleSliderSpinBox::setupConnections() {
  connect(ui->slider, &QSlider::valueChanged, this, &DoubleSliderSpinBox::setSpinBoxValue);
  connect(ui->doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &DoubleSliderSpinBox::setSliderValue);

  connect(ui->doubleSpinBox, &QDoubleSpinBox::valueChanged, this, &DoubleSliderSpinBox::valueChanged);
}

void DoubleSliderSpinBox::setSliderValue(double spin_box_value) {
  ui->slider->setValue(static_cast<int>(spin_box_value / DOUBLE_SLIDER_SPINBOX_PRECISION));
}

void DoubleSliderSpinBox::setSpinBoxValue(int slider_value) {
  ui->doubleSpinBox->setValue(slider_value * DOUBLE_SLIDER_SPINBOX_PRECISION);
}

void DoubleSliderSpinBox::setValue(double value) { ui->doubleSpinBox->setValue(value); }

double DoubleSliderSpinBox::getValue() const { return ui->doubleSpinBox->value(); }

void DoubleSliderSpinBox::setMinimum(double min) {
  ui->slider->setMinimum(static_cast<int>(min / DOUBLE_SLIDER_SPINBOX_PRECISION));
  ui->doubleSpinBox->setMinimum(min);
}

void DoubleSliderSpinBox::setMaximum(double max) {
  ui->slider->setMaximum(static_cast<int>(max / DOUBLE_SLIDER_SPINBOX_PRECISION));
  ui->doubleSpinBox->setMaximum(max);
}

void DoubleSliderSpinBox::setStep(double step) {
  ui->slider->setSingleStep(static_cast<int>(step / DOUBLE_SLIDER_SPINBOX_PRECISION));
  ui->doubleSpinBox->setSingleStep(step);
}

DoubleSliderSpinBox::~DoubleSliderSpinBox() { delete ui; }
