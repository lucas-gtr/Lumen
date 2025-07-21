/**
 * @file SliderSpinBox.hpp
 * @brief Header file for the SliderSpinBox class, which combines a slider and a spin box for selecting integer values.
 */
#ifndef GUI_WIDGETS_SLIDERSPINBOX_HPP
#define GUI_WIDGETS_SLIDERSPINBOX_HPP

#include <QWidget>

namespace Ui {
class SliderSpinBox;
}

/**
 * @class SliderSpinBox
 * @brief A widget that combines a slider and a spin box for selecting integer values.
 *
 * This class provides a user interface element that allows users to select an integer value
 * using both a slider and a spin box. The value can be adjusted using the slider or by typing
 * directly into the spin box.
 */
class SliderSpinBox : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the SliderSpinBox class.
   * @param parent The parent widget for this slider spin box.
   */
  explicit SliderSpinBox(QWidget* parent = nullptr);

  /**
   * @brief Sets the value of the slider spin box.
   * @param value The new integer value to set.
   */
  void setValue(int value);

  /**
   * @brief Gets the current value of the slider spin box.
   * @return The current integer value.
   */
  int getValue() const;

  /**
   * @brief Sets the minimum value for the slider spin box.
   * @param min The minimum integer value.
   */
  void setMinimum(int min);

  /**
   * @brief Sets the maximum value for the slider spin box.
   * @param max The maximum integer value.
   */
  void setMaximum(int max);

  /**
   * @brief Sets the step size for the slider spin box.
   * @param step The step size for incrementing or decrementing the value.
   */
  void setStep(int step);

  ~SliderSpinBox(); ///< Default destructor for the SliderSpinBox class.
signals:
  void valueChanged(int newValue);

private:
  Ui::SliderSpinBox* ui;

  void setupConnections();
};

#endif // GUI_WIDGETS_SLIDERSPINBOX_HPP
