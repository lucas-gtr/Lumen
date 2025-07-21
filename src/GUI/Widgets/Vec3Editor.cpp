#include <linalg/Vec3.hpp>

#include "Vec3Editor.hpp"
#include "ui_Vec3Editor.h"

Vec3Editor::Vec3Editor(QWidget* parent) : QWidget(parent), ui(new Ui::Vec3Editor) {
  ui->setupUi(this);

  connect(ui->xLineEdit, &LineEditDouble::valueChanged, this, &Vec3Editor::onValueChanged);
  connect(ui->yLineEdit, &LineEditDouble::valueChanged, this, &Vec3Editor::onValueChanged);
  connect(ui->zLineEdit, &LineEditDouble::valueChanged, this, &Vec3Editor::onValueChanged);
}

void Vec3Editor::setValue(const linalg::Vec3d& value) {
  ui->xLineEdit->setValue(value.x);
  ui->yLineEdit->setValue(value.y);
  ui->zLineEdit->setValue(value.z);
}

linalg::Vec3d Vec3Editor::getValue() const {
  return {ui->xLineEdit->value(), ui->yLineEdit->value(), ui->zLineEdit->value()};
}

void Vec3Editor::onValueChanged() { emit valueChanged(getValue()); }

Vec3Editor::~Vec3Editor() { delete ui; }