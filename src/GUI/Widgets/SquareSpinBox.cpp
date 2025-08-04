// GCOVR_EXCL_START
#include "SquareSpinBox.hpp"
#include <cmath>

SquareSpinBox::SquareSpinBox(QWidget* parent) : QSpinBox(parent) {
  setSingleStep(1); // Pour que Qt appelle stepBy avec steps = ±1
}

int SquareSpinBox::valueFromText(const QString& text) const {
  bool      ok    = false;
  const int value = text.toInt(&ok);
  if(!ok) {
    return 1;
  }
  const int root = static_cast<int>(std::round(std::sqrt(value)));
  return root * root;
}

QString SquareSpinBox::textFromValue(int value) const { return QString::number(value); }

int SquareSpinBox::nextSquare(int current) {
  const int root = static_cast<int>(std::sqrt(current)) + 1;
  return root * root;
}

int SquareSpinBox::previousSquare(int current) {
  int root = static_cast<int>(std::sqrt(current));
  if(root > 1 || root * root < current) {
    --root;
  }
  return qMax(1, root * root);
}

void SquareSpinBox::stepBy(int steps) {
  const int current   = value();
  int       new_value = current;

  if(steps > 0) {
    for(int i = 0; i < steps; ++i) {
      new_value = nextSquare(new_value);
    }
  } else {
    for(int i = 0; i < -steps; ++i) {
      new_value = previousSquare(new_value);
    }
  }

  setValue(new_value);
}
// GCOVR_EXCL_STOP