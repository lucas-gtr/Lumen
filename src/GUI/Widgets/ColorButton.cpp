#include "ColorButton.hpp"

#include <QColorDialog>
#include <QPainter>

ColorButton::ColorButton(QWidget* parent) : QPushButton(parent), m_current_color{1.0, 1.0, 1.0} {
  updateButtonColor();

  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

  connect(this, &QPushButton::clicked, this, &ColorButton::openColorDialog);
}

void ColorButton::setColor(const ColorRGB& color) {
  if(m_current_color.r == color.r && m_current_color.g == color.g && m_current_color.b == color.b) {
    return;
  }
  m_current_color = color;
  updateButtonColor();
}

ColorRGB ColorButton::getColor() const { return m_current_color; }

void ColorButton::openColorDialog() {
  const QColor initial(static_cast<int>(m_current_color.r * 255), static_cast<int>(m_current_color.g * 255),
                       static_cast<int>(m_current_color.b * 255));

  const QColor selected = QColorDialog::getColor(initial, this, tr("Choose Color"));

  if(selected.isValid()) {
    const ColorRGB new_color{selected.redF(), selected.greenF(), selected.blueF()};
    setColor(new_color);
    emit colorChanged(new_color);
  }
}

void ColorButton::updateButtonColor() {
  const QColor qcolor(static_cast<int>(m_current_color.r * 255), static_cast<int>(m_current_color.g * 255),
                      static_cast<int>(m_current_color.b * 255));

  const QString style = QString("ColorButton {"
                                "background-color: %1;"
                                "border: 2px solid black;"
                                "border-radius: 4px;"
                                "}")
                            .arg(qcolor.name());

  setStyleSheet(style);
}
