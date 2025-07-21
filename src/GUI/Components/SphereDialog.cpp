#include "SphereDialog.hpp"
#include "ui_SphereDialog.h"

SphereDialog::SphereDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SphereDialog) { ui->setupUi(this); }

double SphereDialog::getRadius() const { return ui->radiusSpinBox->value(); }

int SphereDialog::getSegments() const { return ui->segmentsSpinBox->value(); }

int SphereDialog::getRings() const { return ui->ringsSpinBox->value(); }

SphereDialog::~SphereDialog() { delete ui; }
