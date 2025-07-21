/**
 * @file ColorButton.hpp
 * @brief Header file for the ColorButton class, which provides a button to select colors.
 */
#ifndef GUI_WIDGETS_COLORBUTTON_HPP
#define GUI_WIDGETS_COLORBUTTON_HPP

#include <QColor>
#include <QPushButton>

#include "Core/CommonTypes.hpp"

/**
 * @class ColorButton
 * @brief A button that opens a color dialog to select a color.
 *
 * This class inherits from QPushButton and provides functionality to select a color
 * using a QColorDialog. The selected color is stored as a ColorRGB object.
 */
class ColorButton : public QPushButton {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the ColorButton class.
   * @param parent The parent widget for this button.
   */
  explicit ColorButton(QWidget* parent = nullptr);

  /**
   * @brief Sets the current color of the button.
   * @param color The ColorRGB object representing the color to set.
   */
  void setColor(const ColorRGB& color);

  /**
   * @brief Gets the current color of the button.
   * @return The ColorRGB object representing the current color.
   */
  ColorRGB getColor() const;

signals:
  void colorChanged(const ColorRGB& color);

private slots:
  void openColorDialog();

private:
  void updateButtonColor();

  ColorRGB m_current_color;
};

#endif // GUI_WIDGETS_COLORBUTTON_HPP
