#include "SliderSpinBox.hpp"
#include "ui_SliderSpinBox.h"

#include <QSlider>
#include <QSpinBox>

SliderSpinBox::SliderSpinBox(QWidget* parent) : QWidget(parent), ui(new Ui::SliderSpinBox) {
  ui->setupUi(this);
  setupConnections();
}

void SliderSpinBox::setupConnections() {
  connect(ui->slider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
  connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), ui->slider, &QSlider::setValue);

  // Émettre le signal valueChanged lorsque la valeur change
  connect(ui->slider, &QSlider::valueChanged, this, &SliderSpinBox::valueChanged);
}

void SliderSpinBox::setValue(int value) { ui->slider->setValue(value); }

int SliderSpinBox::getValue() const { return ui->slider->value(); }

void SliderSpinBox::setMinimum(int min) {
  ui->slider->setMinimum(min);
  ui->spinBox->setMinimum(min);
}

void SliderSpinBox::setMaximum(int max) {
  ui->slider->setMaximum(max);
  ui->spinBox->setMaximum(max);
}

void SliderSpinBox::setStep(int step) {
  ui->slider->setSingleStep(step);
  ui->spinBox->setSingleStep(step);
}

SliderSpinBox::~SliderSpinBox() { delete ui; }
