// GCOVR_EXCL_START
#include "PlaneDialog.hpp"
#include "ui_PlaneDialog.h"

PlaneDialog::PlaneDialog(QWidget* parent) : QDialog(parent), ui(new Ui::PlaneDialog) { ui->setupUi(this); }

double PlaneDialog::getWidth() const { return ui->widthSpinBox->value(); }

double PlaneDialog::getLength() const { return ui->lengthSpinBox->value(); }

PlaneDialog::~PlaneDialog() { delete ui; }
// GCOVR_EXCL_STOP