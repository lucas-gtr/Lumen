// GCOVR_EXCL_START
#include "LineEditDouble.hpp"
#include "Core/Config.hpp"

LineEditDouble::LineEditDouble(QWidget* parent)
    : QLineEdit(parent), m_validator(new QDoubleValidator(this)), m_minimum(0.0), m_maximum(0.0), m_hasBounds(false) {
  m_validator->setNotation(QDoubleValidator::StandardNotation);
  m_validator->setDecimals(LINE_EDIT_DOUBLE_VALIDATOR_DECIMALS);
  setValidator(m_validator);

  connect(this, &QLineEdit::textChanged, this, &LineEditDouble::onTextChanged);
}

void LineEditDouble::setBounds(double minimum, double maximum) {
  m_minimum   = minimum;
  m_maximum   = maximum;
  m_hasBounds = true;
  m_validator->setRange(minimum, maximum, LINE_EDIT_DOUBLE_VALIDATOR_DECIMALS);
}

double LineEditDouble::value() const { return text().toDouble(); }

void LineEditDouble::setValue(double new_value) {
  setText(QString::number(new_value, 'g', LINE_EDIT_DOUBLE_DISPLAY_PRECISION));
}

void LineEditDouble::onTextChanged(const QString& text) {
  bool         ok = false;
  const double v  = text.toDouble(&ok);
  if(ok) {
    if(!m_hasBounds || (v >= m_minimum && v <= m_maximum)) {
      emit valueChanged(v);
    }
  }
}
// GCOVR_EXCL_STOP