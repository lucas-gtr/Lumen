// GCOVR_EXCL_START
#include "CubeDialog.hpp"
#include "ui_CubeDialog.h"

CubeDialog::CubeDialog(QWidget* parent) : QDialog(parent), ui(new Ui::CubeDialog) { ui->setupUi(this); }

double CubeDialog::getSize() const { return ui->sizeSpinBox->value(); }

CubeDialog::~CubeDialog() { delete ui; }
// GCOVR_EXCL_STOP