/**
 * @file DoubleSliderSpinBox.hpp
 * @brief Header file for the DoubleSliderSpinBox class, which combines a slider and a spin box for selecting double
 * values.
 */
#ifndef GUI_WIDGETS_DOUBLESLIDERSPINBOX_HPP
#define GUI_WIDGETS_DOUBLESLIDERSPINBOX_HPP

#include <QWidget>

namespace Ui {
class DoubleSliderSpinBox;
}

/**
 * @class DoubleSliderSpinBox
 * @brief A widget that combines a slider and a spin box for selecting double values.
 *
 * This class provides a user interface element that allows users to select an double value
 * using both a slider and a spin box. The value can be adjusted using the slider or by typing
 * directly into the spin box.
 */
class DoubleSliderSpinBox : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the DoubleSliderSpinBox class.
   * @param parent The parent widget for this slider spin box.
   */
  explicit DoubleSliderSpinBox(QWidget* parent = nullptr);

  /**
   * @brief Sets the value of the slider spin box.
   * @param value The new double value to set.
   */
  void setValue(double value);

  /**
   * @brief Gets the current value of the slider spin box.
   * @return The current double value.
   */
  double getValue() const;

  /**
   * @brief Sets the minimum value for the slider spin box.
   * @param min The minimum double value.
   */
  void setMinimum(double min);

  /**
   * @brief Sets the maximum value for the slider spin box.
   * @param max The maximum double value.
   */
  void setMaximum(double max);

  /**
   * @brief Sets the step size for the slider spin box.
   * @param step The step size for incrementing or decrementing the value.
   */
  void setStep(double step);

  ~DoubleSliderSpinBox(); ///< Default destructor for the DoubleSliderSpinBox class.
signals:
  void valueChanged(double newValue);

private slots:
  void setSliderValue(double spin_box_value);
  void setSpinBoxValue(int slider_value);

private:
  Ui::DoubleSliderSpinBox* ui;

  void setupConnections();
};

#endif // GUI_WIDGETS_DOUBLESLIDERSPINBOX_HPP
